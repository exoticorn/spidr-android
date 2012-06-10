# Copyright (c) 2009 keen games
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

require 'lace/project'
require 'fileutils'
require 'open3'

module Lace
	class ProjectFileWriterBase
		include PathMixin
		attr_reader :project, :projects, :builds, :build_path
		attr_writer :build_path
		
		def initialize(project_filename, builds,build_dir=nil)
			@lace_bin = to_path($0).dirname
			
			builds=get_available_build_tags(project_filename) if builds.empty?
			
			@builds = builds
			@project_mapping = {}
			@projects = builds.map do |build|
				@project_mapping[build] = Lace::Project.load(project_filename, build.split('/'),build_dir)
			end
			@project = @projects.first
		end
		
		def get_available_build_tags(project_filename)
			# call it once to get the available targets:
			# load the project to get available targets:
			lace_cmd = "ruby #{@lace_bin}/lace #{project_filename}".gsub( /\//, '\\' )

			exitcode = 1
			build_targets = []
		
			status = Open3::popen3( lace_cmd ) do |io_in, io_out, io_err, waitth|			
				io_out.each do |line|  
					if line =~ /build_tags=(.*)/
						build_targets = eval( line.match( /build_tags=(.*)/ )[ 1 ] )								
					else
						puts line
					end
				end  
				io_err.each do |line|
					puts line
				end
				
				exitcode = waitth.value.exitstatus
				return build_targets
			end  
			
			if exitcode != 0 || build_targets.empty?
				puts "No build tags defined in project #{project_file}"
				exit 1
			end
		end
			
		def open_file(filename, &block)
			old_pwd = Dir.getwd
			FileUtils.mkpath(File.dirname(filename))
			Dir.chdir(File.dirname(filename))
			File.open(File.basename(filename), 'w', &block)
			Dir.chdir(old_pwd)
		end
		
		def get_attribute(id, build = nil)
			attributes = []
			(build ? [@project_mapping[build]] : @projects).each do |project|
				project.contexts.each do |context|
					attributes.concat(context.get_local_attributes(id))
				end
			end
			return attributes.uniq
		end
		
		def get_project_files(project)
			files = []
			
			if project then
				files.concat(project.files.map {|f| f.path })
			
				project.sub_projects.each do |subproject|
					files.concat( get_project_files( subproject ) )
				end
			end
			files
		end
		
		def get_files(build = nil)
			files = []
			
			(build ? [@project_mapping[build]] : @projects).each do |project|
				files.concat( get_project_files( project ) )
			end
			return files.uniq
		end
		
		def find_project(build)
			@project_mapping[build]
		end
		
		def build_command(build, jobs = '1',quote_string="")
			build_path_option = ' -p ' + @build_path.to_s if @build_path
			"#{quote_string}#{Helpers::ruby_exe}#{quote_string} #{@lace_bin + 'lace'} -j #{jobs} -b #{build} #{find_project(build).filename}#{build_path_option || ''}"
		end
	end
end
