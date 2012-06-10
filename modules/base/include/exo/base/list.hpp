#ifndef EXO_LIST_HPP
#define EXO_LIST_HPP

#include "exo/base/types.hpp"

namespace exo
{
	class ListBase
	{
	public:
		ListBase();
		~ListBase();

		uint			size() const { return m_size; }

		struct NodeBase
		{
			NodeBase* pNext;
			NodeBase* pPrev;
		};

	protected:
		class IteratorBase
		{
		public:
			IteratorBase(NodeBase* pFirst) : m_pFirst(pFirst) {}
			bool			isAtEnd() const { return m_pFirst == nullptr; }
			void			operator++() { m_pFirst = m_pFirst->pNext; }
		protected:
			NodeBase*	m_pFirst;
			friend class ListBase;
		};

		void			pushBackBase(NodeBase* pNode);
		void			insertBase(const IteratorBase& range, NodeBase* pNode);
		NodeBase*		unlink(NodeBase* pNode);

		uint			m_size;

		NodeBase*		m_pFirst;
		NodeBase*		m_pLast;
	};

	template <typename T>
	class List : public ListBase
	{
	private:
		struct Node : public NodeBase
		{
			Node(T v) : value(v) {}
			T value;
		};
	public:
		class Iterator : public IteratorBase {
		public:
			Iterator(Node* pNode) : IteratorBase(pNode) {}
			T&				operator*() const { return static_cast<Node*>(m_pFirst)->value; }
			T*				operator->() const { return &static_cast<Node*>(m_pFirst)->value; }
		private:
			friend class List<T>;
			Node*			getNode() const { return static_cast<Node*>(m_pFirst); }
		};

		~List()
		{
			Node* pNode = static_cast<Node*>(m_pFirst);
			while(pNode)
			{
				Node* pNext = static_cast<Node*>(pNode->pNext);
				delete pNode;
				pNode = pNext;
			}
		}

		void				pushBack(T value) { pushBackBase(new Node(value)); }
		void				insert(const Iterator& range, T value) { insertBase(range, new Node(value)); }
		Iterator			erase(const Iterator& range)
		{
			Node* pNode = range.getNode();
			Node* pNext = static_cast<Node*>(unlink(pNode));
			delete pNode;
			return Iterator(pNext);
		}

		void				clear() { while(m_pFirst) erase(getIterator()); }

		Iterator			getIterator() const { return Iterator(static_cast<Node*>(m_pFirst)); }
	private:
	};
}

#endif
