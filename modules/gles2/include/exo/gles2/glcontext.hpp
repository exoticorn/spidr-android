#ifndef EXO_GLCONTEXT_HPP
#define EXO_GLCONTEXT_HPP

#include "exo/base/list.hpp"

namespace exo
{
	class GLResource;

	class GLContext
	{
	public:
		void				registerResource(GLResource* pResource);
		void				unregisterResource(GLResource* pResource);

		void				update();
		void				invalidateAll();

	private:
		List<GLResource*>	m_newResources;
		List<GLResource*>	m_existingResources;
	};
}

#endif
