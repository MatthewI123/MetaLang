#pragma once

#include <cassert>
#include "value_list.hpp"

static void test_value_list()
{
	{
		using vl = value_list<>;

		assert((vl::size) == 0);
		assert((vl::push_back<>::size) == 0);
		assert((vl::pop_back<0>::size) == 0);
		assert((vl::push_front<>::size) == 0);
		assert((vl::pop_front<0>::size) == 0);

		assert((vl::insert<0>::size) == 0);
		assert((vl::erase<0, 0>::size) == 0);
		assert((vl::insert<0, 'a'>::size) == 1);
		assert((vl::insert<0, 'a'>::erase<0, 1>::size) == 0);
		
		assert((vl::subsequence<0, 0>::size) == 0);
		assert((vl::insert<0, 'a'>::subsequence<0, 1>::size) == 1);
	}

	{
		using vl = value_list<'b'>;

		assert(('b' == vl::get<0>));
		assert((std::is_same_v<vl, value_list<>::push_back<'b'>>));
		assert((std::is_same_v<vl::set<0, 'a'>, value_list<'a'>>));
		assert((vl::size) == 1);

		assert((vl::push_back<>::size) == 1);
		assert((vl::pop_back<0>::size) == 1);
		assert((vl::push_front<>::size) == 1);
		assert((vl::pop_front<0>::size) == 1);

		assert((vl::push_back<'b'>::size) == 2);
		assert((vl::pop_back<1>::size) == 0);
		assert((vl::push_front<'b'>::size) == 2);
		assert((vl::pop_front<1>::size) == 0);

		assert(('a' == vl::push_front<'a'>::get<0>));
		assert(('b' == vl::push_front<'a'>::get<1>));
		assert(('b' == vl::push_back<'a'>::get<0>));
		assert(('a' == vl::push_back<'a'>::get<1>));

		assert((std::is_same_v<vl, typename vl::push_front<'a'>::push_front<'a'>::subsequence<2, 1>>));
		assert((std::is_same_v<vl, typename vl::push_back<'a'>::push_back<'a'>::subsequence<0, 1>>));
		assert((std::is_same_v<vl, typename vl::push_front<'a'>::push_back<'a'>::subsequence<1, 1>>));

		assert((std::is_same_v<vl, vl::join<vl>::pop_back<vl::size>>));
	}

	assert((make_value_list_t<0, 10>::size) == 10);
}
