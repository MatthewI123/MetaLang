#pragma once

#include <type_traits>
#include "expression.hpp"
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	template<typename Parser>
	struct make_variable_definition
	{
		using next_state_0 = typename Parser::next;
		using next_state_1 = typename next_state_0::template expect<token::decorator::token_kind::identifier>;
		using identifier = typename node::template identifier<typename next_state_0::current>;
		using next_state_2 = typename next_state_1::template expect<token::decorator::token_kind::operator_assign>;
		using operand = make_expression_t<next_state_2>;

		using next_state = typename operand::next_state;
		using node = typename node::template variable_definition<identifier, typename operand::node>;
	};

	template<typename Parser, typename = void>
	struct make_statement
	{
		using type = typename Parser::template expect<token::decorator::token_kind::keyword_let>;
	};

	template<typename Parser>
	struct make_statement<Parser, std::enable_if_t<Parser::template test<token::decorator::token_kind::keyword_let>>>
	{
		using type = make_variable_definition<Parser>;
	};

	template<typename Parser>
	using make_statement_t = typename make_statement<Parser>::type;
}
