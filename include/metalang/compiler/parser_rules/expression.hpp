#pragma once

#include <type_traits>
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	template<typename Parser>
	struct make_unary_expression
	{
		using next_state = Parser;
		using node = int;
	};

	template<typename Parser>
	using make_unary_expression_t = make_unary_expression<Parser>;
}

namespace compiler::parser_rules
{
	template<typename Parser, typename Left, typename = void>
	struct make_multiplicative_expression
	{
		using next_state = Parser;
		using node = Left;
	};

	template<typename Parser, typename Left>
	struct make_multiplicative_expression<Parser, Left,
		std::enable_if_t<(Parser::template test<token::decorator::token_kind::operator_multiply> ||
			Parser::template test<token::decorator::token_kind::operator_divide> ||
			Parser::template test<token::decorator::token_kind::operator_modulo>)>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_unary_expression_t<next_state_0>;
		using type = typename make_multiplicative_expression<typename operand::next_state,
			typename node::template multiplicative_expression<
				((Parser::template test<token::decorator::token_kind::operator_multiply>) ?
					node::decorator::multiplicative_mode::multiply :
						(Parser::template test<token::decorator::token_kind::operator_divide>) ?
							node::decorator::multiplicative_mode::divide :
								node::decorator::multiplicative_mode::modulo), Left,
				typename operand::node>>::type;
	};

	template<typename Parser, typename Left>
	struct make_multiplicative_expression<Parser, Left,
		std::enable_if_t<!(Parser::template test<token::decorator::token_kind::operator_add> ||
			Parser::template test<token::decorator::token_kind::operator_subtract>)>>
	{
		using type = make_multiplicative_expression<Parser, Left, int>; 
	};

	template<typename Parser>
	struct make_multiplicative_expression_helper
	{
		using left = make_unary_expression_t<Parser>;
		using type = typename make_multiplicative_expression<typename left::next_state, typename left::node>::type;
	};

	template<typename Parser>
	using make_multiplicative_expression_t = typename make_multiplicative_expression_helper<Parser>::type;
}

namespace compiler::parser_rules
{
	template<typename Parser, typename Left, typename = void>
	struct make_additive_expression
	{
		using next_state = Parser;
		using node = Left;
	};

	template<typename Parser, typename Left>
	struct make_additive_expression<Parser, Left,
		std::enable_if_t<(Parser::template test<token::decorator::token_kind::operator_add> ||
			Parser::template test<token::decorator::token_kind::operator_subtract>)>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_multiplicative_expression_t<next_state_0>;
		using type = typename make_additive_expression<typename operand::next_state,
			typename node::template additive_expression<
				((Parser::template test<token::decorator::token_kind::operator_add>) ?
					node::decorator::additive_mode::addition : node::decorator::additive_mode::subtraction), Left,
				typename operand::node>>::type;
	};

	template<typename Parser, typename Left>
	struct make_additive_expression<Parser, Left,
		std::enable_if_t<!(Parser::template test<token::decorator::token_kind::operator_add> ||
			Parser::template test<token::decorator::token_kind::operator_subtract>)>>
	{
		using type = make_additive_expression<Parser, Left, int>; 
	};

	template<typename Parser>
	struct make_additive_expression_helper
	{
		using left = make_multiplicative_expression_t<Parser>;
		using type = typename make_additive_expression<typename left::next_state, typename left::node>::type;
	};

	template<typename Parser>
	using make_additive_expression_t = typename make_additive_expression_helper<Parser>::type;

	template<typename Parser>
	using make_expression_t = make_additive_expression_t<Parser>;
}
