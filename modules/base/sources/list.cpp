#include "exo/base/list.hpp"

namespace exo
{
	ListBase::ListBase()
	{
		m_pFirst = nullptr;
		m_pLast = nullptr;
		m_size = 0;
	}

	ListBase::~ListBase()
	{
	}

	void ListBase::pushBackBase(NodeBase* pNode)
	{
		pNode->pPrev = m_pLast;
		pNode->pNext = nullptr;
		if(m_pLast != nullptr)
		{
			m_pLast->pNext = pNode;
		}
		else
		{
			m_pFirst = pNode;
		}
		m_pLast = pNode;
		m_size++;
	}

	void ListBase::insertBase(const IteratorBase& it, NodeBase* pNode)
	{
		NodeBase* pPosition = it.m_pFirst;
		if(pPosition == nullptr)
		{
			pushBackBase(pNode);
			return;
		}

		pNode->pNext = pPosition;
		pNode->pPrev = pPosition->pPrev;
		pPosition->pPrev = pNode;

		if(pNode->pPrev == nullptr)
		{
			m_pFirst = pNode;
		}
		else
		{
			pNode->pPrev->pNext = pNode;
		}
		m_size++;
	}

	ListBase::NodeBase* ListBase::unlink(NodeBase* pNode)
	{
		if(pNode->pPrev == nullptr)
		{
			m_pFirst = pNode->pNext;
		}
		else
		{
			pNode->pPrev->pNext = pNode->pNext;
		}

		if(pNode->pNext == nullptr)
		{
			m_pLast = pNode->pPrev;
		}
		else
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}

		m_size--;

		return pNode->pNext;
	}
}
