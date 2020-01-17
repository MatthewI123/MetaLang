#pragma once

#include <cassert>
#include "type_list.hpp"

static void test_type_list()
{
	{
		using tl = type_list<>;

		assert((tl::size) == 0);
		assert((tl::push_back<>::size) == 0);
		assert((tl::pop_back<0>::size) == 0);
		assert((tl::push_front<>::size) == 0);
		assert((tl::pop_front<0>::size) == 0);

		assert((tl::insert<0>::size) == 0);
		assert((tl::erase<0, 0>::size) == 0);
		assert((tl::insert<0, int>::size) == 1);
		assert((tl::insert<0, int>::erase<0, 1>::size) == 0);
		
		assert((tl::subsequence<0, 0>::size) == 0);
		assert((tl::insert<0, int>::subsequence<0, 1>::size) == 1);
	}

	{
		using tl = type_list<char>;

		assert((std::is_same_v<char, tl::get<0>>));
		assert((std::is_same_v<tl, type_list<>::push_back<char>>));
		assert((std::is_same_v<tl::set<0, int>, type_list<int>>));
		assert((tl::size) == 1);

		assert((tl::push_back<>::size) == 1);
		assert((tl::pop_back<0>::size) == 1);
		assert((tl::push_front<>::size) == 1);
		assert((tl::pop_front<0>::size) == 1);

		assert((tl::push_back<char>::size) == 2);
		assert((tl::pop_back<1>::size) == 0);
		assert((tl::push_front<char>::size) == 2);
		assert((tl::pop_front<1>::size) == 0);

		assert((std::is_same_v<int, tl::push_front<int>::get<0>>));
		assert((std::is_same_v<char, tl::push_front<int>::get<1>>));
		assert((std::is_same_v<char, tl::push_back<int>::get<0>>));
		assert((std::is_same_v<int, tl::push_back<int>::get<1>>));

		assert((std::is_same_v<tl, typename tl::push_front<int>::push_front<int>::subsequence<2, 1>>));
		assert((std::is_same_v<tl, typename tl::push_back<int>::push_back<int>::subsequence<0, 1>>));
		assert((std::is_same_v<tl, typename tl::push_front<int>::push_back<int>::subsequence<1, 1>>));

		assert((std::is_same_v<tl, tl::join<tl>::pop_back<tl::size>>));
	}

	assert((make_type_list_t<char, 10>::size) == 10);
}
