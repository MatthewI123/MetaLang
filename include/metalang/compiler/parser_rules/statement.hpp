#pragma once

#include <type_traits>
#include "expression.hpp"
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	template<typename Parser, typename Parameters>
	struct make_parameters
	{
		using next_state = Parser;
		using list = Parameters;
	};

	template<typename Parser, typename Parameters, typename = void>
	struct make_parameters_helper
	{
		using next_state_0 = typename Parser::template expect<token::decorator::token_kind::symbol_delimiter>;
		using parameter = typename node::template identifier<typename next_state_0::current>;
		using type = typename make_parameters_helper<typename next_state_0::next,
			typename Parameters::template push_back<parameter>>::type;
	};

	template<typename Parser, typename Parameters>
	struct make_parameters_helper<Parser, Parameters,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::symbol_right_parenthesis>>
	{
		using type = make_parameters<typename Parser::next, Parameters>;
	};

	template<typename Parser, typename = void>
	struct make_parameters_dispatch
	{
		using next_state_0 = typename Parser::template expect<token::decorator::token_kind::identifier>;
		using identifier = typename node::template identifier<typename Parser::current>;
		using type = typename make_parameters_helper<next_state_0, type_list<identifier>>::type;
	};

	template<typename Parser>
	struct make_parameters_dispatch<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::symbol_right_parenthesis>>
	{
		using type = make_parameters<typename Parser::next, type_list<>>;
	};

	template<typename Parser, typename Identifier>
	struct make_variable_definition
	{
		using operand = make_expression_t<Parser>;
	
		using next_state = typename operand::next_state;
		using node = typename node::template variable_definition<Identifier, typename operand::node>;
	};

	template<typename Parser, typename Identifier>
	struct make_function_definition
	{
		using parameters = typename make_parameters_dispatch<Parser>::type;
		using next_state_0 = typename parameters::next_state::template expect<
			token::decorator::token_kind::operator_assign>;
		using operand = make_expression_t<next_state_0>;

		using next_state = typename operand::next_state;
		using node = typename node::template function_definition<Identifier, typename parameters::list,
			typename operand::node>;
	};

	template<typename Parser, typename Identifier, typename = void>
	struct make_statement
	{
		using type = typename Parser::template expect<token::decorator::token_kind::operator_assign>;
	};

	template<typename Parser, typename Identifier>
	struct make_statement<Parser, Identifier,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_assign>>
	{
		using next_state_0 = typename Parser::next;
		using type = make_variable_definition<next_state_0, Identifier>;
	};

	template<typename Parser, typename Identifier>
	struct make_statement<Parser, Identifier,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::symbol_left_parenthesis>>
	{
		using next_state_0 = typename Parser::next;
		using type = make_function_definition<next_state_0, Identifier>;
	};

	template<typename Parser, typename = void>
	struct make_statement_dispatch
	{
		using type = typename Parser::template expect<token::decorator::token_kind::keyword_let>;
	};

	template<typename Parser>
	struct make_statement_dispatch<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::keyword_let>>
	{
		using next_state_0 = typename Parser::next;
		using next_state_1 = typename next_state_0::template expect<token::decorator::token_kind::identifier>;
		using identifier = typename node::template identifier<typename next_state_0::current>;
		using type = typename make_statement<next_state_1, identifier>::type;
	};

	template<typename Parser>
	using make_statement_t = typename make_statement_dispatch<Parser>::type;
}
