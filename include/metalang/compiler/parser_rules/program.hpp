#pragma once

#include <type_traits>
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"
#include "statement.hpp"

namespace compiler::parser_rules
{
	// base case (no more statements)
	template<typename Parser, typename Node, typename = void>
	struct make_program
	{
		using next_state = Parser;
		using node = Node;
	};

	template<typename Parser, typename Node>
	struct make_program<Parser, Node, std::enable_if_t<Parser::done>>
	{
		using type = make_program<Parser, Node, int>;
	};

	template<typename Parser, typename Node>
	struct make_program<Parser, Node, std::enable_if_t<!Parser::done>>
	{
		using statement = make_statement_t<Parser>;
		using type = typename make_program<
			typename statement::next_state,
			typename Node::template add_statement<typename statement::node>
		>::type;
	};

	template<typename Parser>
	using make_program_t = typename make_program<Parser, typename node::template program<type_list<>>>::type;
}
