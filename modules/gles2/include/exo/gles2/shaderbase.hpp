#ifndef EXO_SHADERBASE_HPP
#define EXO_SHADERBASE_HPP

#include "exo/gles2/glresource.hpp"
#include "exo/base/types.hpp"
#include "exo/gles2/gl.h"

namespace exo
{
	class GLContext;
	
	class ShaderBase : public GLResource
	{
	public:
		struct SourceData
		{
			const char* pVertexSource;
			const char* pFragmentSource;
			uint numAttributes;
			const char** ppAttributeNames;
			uint numUniforms;
			const char** ppUniformNames;
		};
	
		ShaderBase(GLContext& context, const SourceData& sourceData);
		~ShaderBase();
		
		virtual void	createGLResource();

		void			use();
		
	protected:
		const SourceData&	m_sourceData;
		GLint				m_shaderProgram;
		GLint*				m_attributes;
		GLint*				m_uniforms;
	};
}

#endif

