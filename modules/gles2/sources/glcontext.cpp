#include "exo/gles2/glcontext.hpp"
#include "exo/gles2/glresource.hpp"
#include "exo/base/algorithm.hpp"

namespace exo
{
	void GLContext::registerResource(GLResource* pResource)
	{
		m_newResources.pushBack(pResource);
	}

	void GLContext::unregisterResource(GLResource* pResource)
	{
		List<GLResource*>::Iterator it = find(m_newResources.getIterator(), pResource);
		if(!it.isAtEnd())
		{
			m_newResources.erase(it);
		}
		else
		{
			it = find(m_existingResources.getIterator(), pResource);
			if(!it.isAtEnd())
			{
				m_existingResources.erase(it);
			}
		}
	}

	void GLContext::update()
	{
		for(List<GLResource*>::Iterator it = m_newResources.getIterator(); !it.isAtEnd();)
		{
			(*it)->createGLResource();
			(*it)->m_isGLResourceCreated = true;
			m_existingResources.pushBack(*it);
			it = m_newResources.erase(it);
		}
	}

	void GLContext::invalidateAll()
	{
		for(List<GLResource*>::Iterator it = m_existingResources.getIterator(); !it.isAtEnd();)
		{
			m_newResources.pushBack(*it);
			it = m_existingResources.erase(it);
		}
	}
}
