#pragma once

#include <cstdint>
#include <type_traits>

template<typename... Types>
class type_list
{
private:
	template<std::size_t, typename...>
	struct get_impl { };

	template<std::size_t Index, typename Head, typename... Tail>
	struct get_impl<Index, Head, Tail...> : get_impl<Index - 1, Tail...> { };

	template<typename Head, typename... Tail>
	struct get_impl<0, Head, Tail...>
	{
		using type = Head;
	};

	template<typename TypeList1, typename TypeList2>
	struct join_impl { };

	template<typename... Types1, typename... Types2>
	struct join_impl<type_list<Types1...>, type_list<Types2...>>
	{
		using type = type_list<Types1..., Types2...>;
	};

	template<std::size_t Index, std::size_t Count, typename... Type>
	struct subsequence_impl
	{
		using type = typename join_impl<
			type_list<typename get_impl<Index, Type...>::type>,
			typename subsequence_impl<Index + 1, Count - 1, Type...>::type>::type;
	};

	template<std::size_t Index, typename... Type>
	struct subsequence_impl<Index, 0, Type...>
	{
		using type = type_list<>;
	};

public:
	using type = type_list<Types...>;

	static constexpr std::size_t size = sizeof...(Types);

	template<std::size_t Index, std::size_t Count>
	using subsequence = typename subsequence_impl<Index, Count, Types...>::type;

	template<typename... Type>
	using push_front = type_list<Type..., Types...>;

	template<typename... Type>
	using push_back = type_list<Types..., Type...>;
	
	template<std::size_t Count = 1>
	using pop_front = subsequence<Count, size - Count>;

	template<std::size_t Count = 1>
	using pop_back = subsequence<0, size - Count>;

	template<typename TypeList>
	using join = typename join_impl<type, TypeList>::type;
	
	template<std::size_t Index>
	using get = typename get_impl<Index, Types...>::type;
	
	template<std::size_t Index, typename Type>
	using set = typename subsequence<0, Index>::template push_back<Type>
		::template join<subsequence<Index + 1, size - Index - 1>>;

	template<std::size_t Index, typename... Type>
	using insert = typename subsequence<0, Index>::template push_back<Type...>
		::template join<subsequence<Index, size - Index>>;
	
	template<std::size_t Index, std::size_t Count = 1>
	using erase = typename subsequence<0, Index>::template join<subsequence<Index + Count, size - Index - Count>>;
};

template<typename Type, std::size_t Count, typename TypeList = type_list<>>
struct make_type_list : make_type_list<Type, Count - 1, typename TypeList::template push_back<Type>> { };

template<typename Type, typename TypeList>
struct make_type_list<Type, 0, TypeList>
{
	using type = TypeList;
};

template<typename Type, std::size_t Count>
using make_type_list_t = typename make_type_list<Type, Count>::type;
