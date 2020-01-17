#pragma once

#include <cstdint>
#include <type_traits>

template<auto... Values>
class value_list
{
private:
	template<std::size_t, auto...>
	struct get_impl { };

	template<std::size_t Index, auto Head, auto... Tail>
	struct get_impl<Index, Head, Tail...> : get_impl<Index - 1, Tail...> { };

	template<auto Head, auto... Tail>
	struct get_impl<0, Head, Tail...>
	{
		static constexpr auto value = Head;
	};

	template<typename ValueList1, typename ValueList2>
	struct join_impl { };

	template<auto... Values1, auto... Values2>
	struct join_impl<value_list<Values1...>, value_list<Values2...>>
	{
		using type = value_list<Values1..., Values2...>;
	};

	template<std::size_t Index, std::size_t Count, auto... Value>
	struct subsequence_impl
	{
		using type = typename join_impl<
			value_list<get_impl<Index, Value...>::value>,
			typename subsequence_impl<Index + 1, Count - 1, Value...>::type>::type;
	};

	template<std::size_t Index, auto... Value>
	struct subsequence_impl<Index, 0, Value...>
	{
		using type = value_list<>;
	};

public:
	using type = value_list<Values...>;

	static constexpr std::size_t size = sizeof...(Values);

	template<std::size_t Index, std::size_t Count>
	using subsequence = typename subsequence_impl<Index, Count, Values...>::type;

	template<auto... Value>
	using push_front = value_list<Value..., Values...>;

	template<auto... Value>
	using push_back = value_list<Values..., Value...>;
	
	template<std::size_t Count = 1>
	using pop_front = subsequence<Count, size - Count>;

	template<std::size_t Count = 1>
	using pop_back = subsequence<0, size - Count>;

	template<typename ValueList>
	using join = typename join_impl<type, ValueList>::type;
	
	template<std::size_t Index>
	static constexpr auto get = get_impl<Index, Values...>::value;
	
	template<std::size_t Index, auto Value>
	using set = typename subsequence<0, Index>::template push_back<Value>
		::template join<subsequence<Index + 1, size - Index - 1>>;

	template<std::size_t Index, auto... Value>
	using insert = typename subsequence<0, Index>::template push_back<Value...>
		::template join<subsequence<Index, size - Index>>;
	
	template<std::size_t Index, std::size_t Count = 1>
	using erase = typename subsequence<0, Index>::template join<subsequence<Index + Count, size - Index - Count>>;
};

template<auto Value, std::size_t Count, typename ValueList = value_list<>>
struct make_value_list : make_value_list<Value, Count - 1, typename ValueList::template push_back<Value>> { };

template<auto Value, typename ValueList>
struct make_value_list<Value, 0, ValueList>
{
	using type = ValueList;
};

template<auto Value, std::size_t Count>
using make_value_list_t = typename make_value_list<Value, Count>::type;
