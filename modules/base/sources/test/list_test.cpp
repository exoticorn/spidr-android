#include "exo/unittest.hpp"
#include "exo/base/list.hpp"
#include "exo/base/algorithm.hpp"

using namespace exo;

UNIT_TEST(list_size)
{
	List<int> list;
	UT_ASSERT_EQ(list.size(), 0);
	list.pushBack(1);
	UT_ASSERT_EQ(list.size(), 1);
}

UNIT_TEST(list_iterator)
{
	List<int> list;
	UT_ASSERT(list.getIterator().isAtEnd());
	list.pushBack(1);
	UT_ASSERT(!list.getIterator().isAtEnd());
	UT_ASSERT_EQ(*list.getIterator(), 1);
	list.pushBack(2);
	List<int>::Iterator it = list.getIterator();
	UT_ASSERT_EQ(*it, 1);
	++it;
	UT_ASSERT_EQ(*it, 2);
}

UNIT_TEST(list_insert)
{
	List<int> list;
	list.pushBack(1);
	list.pushBack(2);
	list.insert(find(list.getIterator(), 2), 3);
	list.insert(list.getIterator(), 4);
	list.insert(find(list.getIterator(), 99), 5);
	UT_ASSERT_EQ(list.size(), 5);
	List<int>::Iterator it = list.getIterator();
	UT_ASSERT_EQ(*it, 4);
	++it;
	UT_ASSERT_EQ(*it, 1);
	++it;
	UT_ASSERT_EQ(*it, 3);
	++it;
	UT_ASSERT_EQ(*it, 2);
	++it;
	UT_ASSERT_EQ(*it, 5);
	++it;
	UT_ASSERT(it.isAtEnd());
}

UNIT_TEST(list_erase)
{
	List<int> list;
	list.pushBack(1);
	list.pushBack(2);
	list.pushBack(3);
	List<int>::Iterator it = list.erase(find(list.getIterator(), 2));
	UT_ASSERT_EQ(*it, 3);
	UT_ASSERT_EQ(list.size(), 2);
	it = list.getIterator();
	UT_ASSERT_EQ(*it, 1);
	++it;
	UT_ASSERT_EQ(*it, 3);
	++it;
	UT_ASSERT(it.isAtEnd());
}

UNIT_TEST(list_clear)
{
	List<int> list;
	list.pushBack(1);
	list.pushBack(2);
	list.clear();
	UT_ASSERT_EQ(list.size(), 0);
	UT_ASSERT(list.getIterator().isAtEnd());
}
