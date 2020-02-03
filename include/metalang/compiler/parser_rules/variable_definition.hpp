#pragma once

#include <type_traits>
#include "expression.hpp"
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	// let <identifier> = <expression>
	//                    ^
	template<typename Parser, typename Identifier>
	class make_variable_definition
	{
	private:
		using operand = make_expression_t<Parser>;

	public:
		using next_state = typename operand::next_state;
		using node = typename node::template variable_definition<Identifier, typename operand::node>;
	};

	template<typename Parser, typename Identifier>
	using make_variable_definition_t = make_variable_definition<Parser, Identifier>;
}
