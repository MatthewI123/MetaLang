#pragma once

#include <cstdint>
#include <type_traits>

/**
 * A type_list is a compile-time sequence of types using variadic template type parameters.
 */
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
	// The current type_list.
	using type = type_list<Types...>;

	// The number of types.
	static constexpr std::size_t size = sizeof...(Types);

	// A type_list consisting of count types from the given index.
	template<std::size_t Index, std::size_t Count>
	using subsequence = typename subsequence_impl<Index, Count, Types...>::type;

	// A type_list with the given type arguments prepended.
	template<typename... Type>
	using push_front = type_list<Type..., Types...>;

	// A type_list with the given type arguments appended.
	template<typename... Type>
	using push_back = type_list<Types..., Type...>;
	
	// A type_list with the given number of types removed from the front.
	template<std::size_t Count = 1>
	using pop_front = subsequence<Count, size - Count>;

	// A type_list with the given number of types removed from the back.
	template<std::size_t Count = 1>
	using pop_back = subsequence<0, size - Count>;

	// A type_list with all types of the given type_list appended.
	template<typename TypeList>
	using join = typename join_impl<type, TypeList>::type;
	
	// The type at the given index.
	template<std::size_t Index>
	using get = typename get_impl<Index, Types...>::type;
	
	// A type_list with the type at the given index replaced with another type.
	template<std::size_t Index, typename Type>
	using set = typename subsequence<0, Index>::template push_back<Type>
		::template join<subsequence<Index + 1, size - Index - 1>>;

	// A type_list with the given types inserted at the given index.
	template<std::size_t Index, typename... Type>
	using insert = typename subsequence<0, Index>::template push_back<Type...>
		::template join<subsequence<Index, size - Index>>;
	
	// A type_list with count types removed from the given index.
	template<std::size_t Index, std::size_t Count = 1>
	using erase = typename subsequence<0, Index>::template join<subsequence<Index + Count, size - Index - Count>>;
};

// Helper to yield a type_list populated with given type.
template<typename Type, std::size_t Count, typename TypeList = type_list<>>
struct make_type_list : make_type_list<Type, Count - 1, typename TypeList::template push_back<Type>> { };

template<typename Type, typename TypeList>
struct make_type_list<Type, 0, TypeList>
{
	using type = TypeList;
};

// Helper to yield a type_list populated with given type.
template<typename Type, std::size_t Count>
using make_type_list_t = typename make_type_list<Type, Count>::type;
