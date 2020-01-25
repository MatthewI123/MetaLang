#pragma once

#include <type_traits>
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"


namespace compiler::parser_rules
{
	template<typename Parser>
	struct make_additive_expression_dispatch;

	template<typename Parser>
	using make_expression_t = typename make_additive_expression_dispatch<Parser>::type;

	template<typename Parser, typename Node>
	struct make_primary_expression
	{
		using next_state = Parser;
		using node = Node;
	};

	template<typename Parser, typename = void>
	struct make_primary_expression_helper
	{
		using type = typename Parser::template unexpected<>;
	};

	template<typename Parser>
	struct make_primary_expression_helper<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::integer>>
	{
		using node = typename node::template integer<typename Parser::current>;
		using next_state_0 = typename Parser::next;
		using type = make_primary_expression<next_state_0, node>;
	};

	template<typename Parser>
	struct make_primary_expression_helper<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::identifier>>
	{
		using node = typename node::template identifier<typename Parser::current>;
		using next_state_0 = typename Parser::next;
		using type = make_primary_expression<next_state_0, node>;
	};

	template<typename Parser>
	struct make_primary_expression_helper<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::symbol_left_parenthesis>>
	{
		using next_state_0 = typename Parser::next;
		using expression = make_expression_t<next_state_0>;
		using next_state_1 = typename expression::next_state;
		using node = typename expression::node;
		using next_state_2 = typename next_state_1::template expect<
			token::decorator::token_kind::symbol_right_parenthesis>;
		using type = make_primary_expression<next_state_2, node>;
	};

	template<typename Parser>
	using make_primary_expression_t = typename make_primary_expression_helper<Parser>::type;
}

namespace compiler::parser_rules
{
	template<typename Parser, typename Node>
	struct make_unary_expression
	{
		using next_state = Parser;
		using node = Node;
	};

	template<typename Parser, typename = void>
	struct make_unary_expression_helper
	{
		using operand = make_primary_expression_t<Parser>;
		using type = make_unary_expression<typename operand::next_state, typename operand::node>;
	};

	template<typename Parser>
	struct make_unary_expression_helper<Parser,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_subtract>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_primary_expression_t<next_state_0>;
		using type = make_unary_expression<typename operand::next_state,
			typename node::template unary_expression<node::decorator::unary_mode::negation, typename operand::node>>;
	};

	template<typename Parser>
	using make_unary_expression_t = typename make_unary_expression_helper<Parser>::type;
}

namespace compiler::parser_rules
{
	template<token::decorator::token_kind Kind>
	constexpr bool is_multiplicative = Kind == token::decorator::token_kind::operator_multiply ||
		Kind == token::decorator::token_kind::operator_divide || Kind == token::decorator::token_kind::operator_modulo;

	template<typename Parser, typename Left>
	struct make_multiplicative_expression
	{
		using next_state = Parser;
		using node = Left;
	};

	template<typename Parser, typename Left, typename = void>
	struct make_multiplicative_expression_helper
	{
		using type = make_multiplicative_expression<Parser, Left>; 
	};

	template<typename Parser, typename Left>
	struct make_multiplicative_expression_helper<Parser, Left,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_multiply>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_unary_expression_t<next_state_0>;
		using type = typename make_multiplicative_expression_helper<typename operand::next_state,
			typename node::template multiplicative_expression<node::decorator::multiplicative_mode::multiply, Left,
				typename operand::node>>::type;
	};

	template<typename Parser, typename Left>
	struct make_multiplicative_expression_helper<Parser, Left,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_divide>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_unary_expression_t<next_state_0>;
		using type = typename make_multiplicative_expression_helper<typename operand::next_state,
			typename node::template multiplicative_expression<node::decorator::multiplicative_mode::divide, Left,
				typename operand::node>>::type;
	};

	template<typename Parser, typename Left>
	struct make_multiplicative_expression_helper<Parser, Left,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_modulo>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_unary_expression_t<next_state_0>;
		using type = typename make_multiplicative_expression_helper<typename operand::next_state,
			typename node::template multiplicative_expression<node::decorator::multiplicative_mode::modulo, Left,
				typename operand::node>>::type;
	};

	template<typename Parser>
	struct make_multiplicative_expression_dispatch
	{
		using left = make_unary_expression_t<Parser>;
		using type = typename make_multiplicative_expression_helper<typename left::next_state,
			typename left::node>::type;
	};

	template<typename Parser>
	using make_multiplicative_expression_t = typename make_multiplicative_expression_dispatch<Parser>::type;
}

namespace compiler::parser_rules
{
	template<token::decorator::token_kind Kind>
	constexpr bool is_additive = Kind == token::decorator::token_kind::operator_add ||
		Kind == token::decorator::token_kind::operator_subtract;

	template<typename Parser, typename Left>
	struct make_additive_expression
	{
		using next_state = Parser;
		using node = Left;
	};

	template<typename Parser, typename Left, typename = void>
	struct make_additive_expression_helper
	{
		using type = make_additive_expression<Parser, Left>; 
	};

	template<typename Parser, typename Left>
	struct make_additive_expression_helper<Parser, Left,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_add>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_multiplicative_expression_t<next_state_0>;
		using type = typename make_additive_expression_helper<typename operand::next_state,
			typename node::template additive_expression<node::decorator::additive_mode::addition, Left,
				typename operand::node>>::type;
	};

	template<typename Parser, typename Left>
	struct make_additive_expression_helper<Parser, Left,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::operator_subtract>>
	{
		using next_state_0 = typename Parser::next;
		using operand = make_multiplicative_expression_t<next_state_0>;
		using type = typename make_additive_expression_helper<typename operand::next_state,
			typename node::template additive_expression<node::decorator::additive_mode::subtraction, Left,
				typename operand::node>>::type;
	};

	template<typename Parser>
	struct make_additive_expression_dispatch
	{
		using left = make_multiplicative_expression_t<Parser>;
		using type = typename make_additive_expression_helper<typename left::next_state,
			typename left::node>::type;
	};

	template<typename Parser>
	using make_additive_expression_t = typename make_additive_expression_dispatch<Parser>::type;
}
