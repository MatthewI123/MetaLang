#pragma once

#include <type_traits>
#include "node.hpp"

namespace compiler
{
	template<typename State, typename Node, typename = void>
	struct reduce;

	template<typename State, typename Node>
	struct reduce<Node, std::enable_if_t<Node::kind == node::decorator::node_kind::additive_expression>>
	{
		static constexpr auto value = reduce<typename Node::left>::value + reduce<typename Node::right>::value;
		using next_state = State;
	};

	template<typename State, typename Node>
	struct reduce<Node, std::enable_if_t<Node::kind == node::decorator::node_kind::additive_expression>>
	{
		static constexpr auto value = reduce<typename Node::left>::value + reduce<typename Node::right>::value;
		using next_state = State;
	};
}
