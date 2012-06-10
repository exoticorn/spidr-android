require 'rexml/document'
require 'English'

class Vector2
	attr_accessor :x, :y
	
	def initialize(x, y)
		@x, @y = x, y
	end
	
	def *(o)
		if o.is_a?(Numeric)
			Vector2.new(@x * o, @y * o)
		else
			@x * o.x + @y * o.y
		end
	end
	
	def +(o)
		Vector2.new(@x + o.x, @y + o.y)
	end

	def -(o)
		Vector2.new(@x - o.x, @y - o.y)
	end
	
	def ==(o)
		@x == o.x && @y == o.y
	end
	
	def length
		Math::sqrt(self * self)
	end
	
	def normalize
		self * (1 / length)
	end
	
	def dup
		Vector2.new(@x, @y)
	end
end

class Matrix32
	attr_accessor :x, :y, :pos
	
	def initialize(x = Vector2.new(1, 0), y = Vector2.new(0, 1), pos = Vector2.new(0, 0))
		@x, @y, @pos = x, y, pos
	end
	
	def mul_vector(vec)
		@x * vec.x + @y * vec.y + @pos
	end
	
	def rot_vector(vec)
		@x * vec.x + @y * vec.y
	end
	
	def mul_matrix(other)
		Matrix32.new(rot_vector(other.x), rot_vector(other.y), mul_vector(other.pos))
	end
	
	def dup
		Matrix32.new(@x.dup, @y.dup, @pos.dup)
	end
end

class PathLexer
	def initialize(path)
		@path = path.gsub(',', ' ').strip
		@next = nil
		@relative = false
	end
	
	def get_next
		if @next
			result = @next
			@next = nil
		elsif @path =~ /^([MmZzLlCc]|(\-?\d+(\.\d+)?))\s*/
			result = $1
			@path = $POSTMATCH
		else
			raise sprintf("Unable to lex path at '%s'\n", @path[0, 10])
		end
		@relative = (result =~ /[a-z]/) if result =~ /[A-Za-z]/
		return result
	end
	
	def is_param?
		return false if eof?
		result = get_next
		@next = result
		return result !~ /[A-Za-z]/
	end
	
	def get_command
		result = get_next
		raise "Command expected" unless result =~ /[A-Za-z]/
		return result
	end
	
	def get_parameter
		result = get_next
		raise "Parameter expected" if result =~ /[A-Za-z]/
		return result.to_f
	end
	
	def get_vector(current)
		x = get_parameter
		y = get_parameter
		if @relative
			return Vector2.new(x, y) + current
		else
			return Vector2.new(x, y)
		end
	end
	
	def eof?
		@path.empty?
	end
end

def subdivide_curve(p1, p2, p3, p4)
	error = 2 - (p4-p1).normalize * (p2-p1).normalize - (p4-p1).normalize * (p4-p3).normalize
	if error < 0.05 || error.nan?
		return [[p1, p4]]
	else
		m1 = (p1+p2) * 0.5
		m2 = (p2+p3) * 0.5
		m3 = (p3+p4) * 0.5
		m12 = (m1+m2) * 0.5
		m23 = (m2+m3) * 0.5
		m = (m12+m23) * 0.5
		return subdivide_curve(p1, m1, m12, m) + subdivide_curve(m, m23, m3, p4)
	end
end

def read_path(path)
	cur = Vector2.new(0, 0)
	lines = []
	lexer = PathLexer.new(path)
	subpath_start = nil
	until lexer.eof?
		case lexer.get_command
		when 'M', 'm'
			subpath_start = cur = lexer.get_vector(cur)
			while lexer.is_param?
				new = lexer.get_vector(cur)
				lines << [cur, new]
				cur = new
			end
		when 'Z', 'z'
			if cur != subpath_start
				lines << [cur, subpath_start]
			end
		when 'L', 'l'
			while lexer.is_param?
				new = lexer.get_vector(cur)
				lines << [cur, new]
				cur = new
			end
		when 'C', 'c'
			while lexer.is_param?
				c1 = lexer.get_vector(cur)
				c2 = lexer.get_vector(cur)
				new = lexer.get_vector(cur)
				lines.concat(subdivide_curve(cur, c1, c2, new))
				cur = new
			end
		else
			raise "Unsupported command"
		end
	end
	return lines
end

def hatch_area(area, spacing)
	normal = Vector2.new(1, 2).normalize
	tangent = Vector2.new(normal.y, -normal.x)
	dots = area.flatten.map {|v| v * normal }
	lines = []
	((dots.min + spacing * 0.5)...dots.max).step(spacing) do |d|
		points = []
		area.each do |p1, p2|
			side1 = p1 * normal > d
			side2 = p2 * normal > d
			if side1 != side2
				dot1 = p1 * normal
				dot2 = p2 * normal
				points << p1 + (p2 - p1) * ((d - dot1) / (dot2 - dot1))
			end
		end
		if (points.size % 2) == 0
			points = points.sort_by {|p| p * tangent }
			(0...points.size).step(2) do |i|
				lines << points[i, 2]
			end
		end
	end
	return lines
end

Helper = Struct.new :name, :position
Gfx = Struct.new :outline, :hatching
GfxObject = Struct.new :name, :outline, :hatching

def get_color(string, location)
	if string =~ Regexp.new(location + '#(\\w+)')
		r, g, b = 0
		col = $1
		if col =~ /^(\w\w)(\w\w)(\w\w)$/
			r, g, b = $1.to_i(16), $2.to_i(16), $3.to_i(16)
		elsif col =~ /^(\w)(\w)(\w)$/
			r, g, b = $1.to_i(16) * 17, $2.to_i(16) * 17, $3.to_i(16) * 17
		end
		r = r > 128 ? 1 : 0
		g = g > 128 ? 1 : 0
		b = b > 128 ? 1 : 0
		return "#{r}#{g}#{b}"
	end
	return '000'
end

def read_svg_rec(node, matrix, &block)
	matrix = matrix.dup
	transform = node.attributes['transform']
	if transform
		transform.scan(/(\w+)\(([^)]+)\)/) do |op, param|
			params = param.split(',').map {|v| v.to_f}
			case op
			when 'translate'
				vec = Vector2.new(*params)
				matrix.pos += vec
			when 'matrix'
				m = Matrix32.new(Vector2.new(*params[0, 2]), Vector2.new(*params[2, 2]), Vector2.new(*params[4, 2]))
				matrix = matrix.mul_matrix(m)
			else
				printf "Unsupported transform op '%s'\n", op
			end
		end
	end
	
	outline = []
	hatching = []
	node.elements.each('g') do |group|
		id = group.attributes['id']
	
		if id =~ /^obj-(\w+)$/
			name = $1
			read_svg_rec(group, matrix) do |obj|
				block.call(GfxObject.new(name, obj.outline, obj.hatching))
			end
		else
			read_svg_rec(group, matrix) do |obj|
				if obj.is_a?(Gfx)
					outline.concat(obj.outline)
					hatching.concat(obj.hatching)
				else
					block.call(obj)
				end
			end
		end
	end
	
	node.elements.each('path') do |path|
		path_lines = read_path(path.attributes['d'])
		path_lines.map! {|p1, p2| [matrix.mul_vector(p1), matrix.mul_vector(p2)] }
		style = path.attributes['style']
		if style !~ /fill:none/
			hatching.concat(hatch_area(path_lines, $hatching))
		else
			color = get_color(style, 'stroke:')
			if color == '001'
				hatching.concat(path_lines)
			else
				outline.concat(path_lines)
			end
		end
	end
	
	node.elements.each('rect') do |rect|
		id = rect.attributes['id']
	
		x = rect.attributes['x'].to_f + rect.attributes['width'].to_f * 0.5
		y = rect.attributes['y'].to_f + rect.attributes['height'].to_f * 0.5
		pos = matrix.mul_vector(Vector2.new(x, y))
		if id =~ /^helper-(\w+)$/
			name = $1
			block.call(Helper.new(name, pos))
		elsif rect.attributes['style'] =~ /fill:#(\w\w)(\w\w)(\w\w)/
			r, g, b = $1.to_i(16), $2.to_i(16), $3.to_i(16)
			r = r > 128 ? 1 : 0
			g = g > 128 ? 1 : 0
			b = b > 128 ? 1 : 0
			unless r == g && g == b
				block.call(Helper.new("#{r}#{g}#{b}", pos))
			end
		end
	end
	
	unless outline.empty? && hatching.empty?
		block.call(Gfx.new(outline, hatching))
	end
end

def read_svg(filename, &block)
	doc = REXML::Document.new(File.read(filename))
	
	matrix = Matrix32.new(Vector2.new(0.01, 0), Vector2.new(0, 0.01), Vector2.new(0, 0))
	read_svg_rec(doc.elements['svg'], matrix, &block)
end


