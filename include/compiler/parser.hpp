#pragma once

#include <type_traits>
#include "token.hpp"
#include "node.hpp"
#include "type_list.hpp"

namespace compiler
{
	template<typename Tokens>
	struct parser;

	template<typename Parser, token::decorator::token_kind Kind>
	struct parser_error_expected
	{
		using parser = Parser;
		static constexpr auto kind = Kind;

		using token = typename parser::token;
	};

	template<typename Parser, token::decorator::token_kind Kind>
	struct parser_error_unexpected
	{
		using parser = Parser;
		static constexpr auto kind = Kind;

		using token = typename parser::token;
	};

	namespace parse_rules
	{
		// expression = additive_expression '+' multiplicative_expression   |  mul_expr
		namespace
		{
/*			template<typename Parser, typename Left, node::decorator::additive_mode Mode, typename>
			struct additive_expression;

			template<typename Parser, typename Left, node::decorator::additive_mode Mode>
			struct additive_expression<Parser, Left, Mode, void>
			{
				using next_state_0 = typename Parser::pop_front;
				using left = Left;
				using right = make_multiplicative_expression_t<next_state_0>;
				using next_state = typename right::next_state;
				using node = typename node::template additive_expression<Mode, typename left::node,
					typename right::node>;
			};

			template<typename Parser>
			struct additive_expression_dispatch
			{
				using left = make_multiplicative_expression_t<Parser>;
				using next_state = typename left::next_state;
				using type = std::conditional_t<
					(next_state_0::token::kind == token::decorator::token_kind::operator_add),
					typename additive_expression<next_state, left, node::decorator::additive_mode::addition>::type,
					std::conditional_t<(next_state::token::kind == token::decorator::token_kind::operator_subtract),
						typename
							additive_expression<next_state, left, node::decorator::additive_mode::subtraction>::type,
						left>>>;
			};

			template<typename Parser>
			using make_additive_expression_t = typename additive_expression_dispatch<Parser>::type;

			template<typename Parser>
			using make_expression_t = make_additive_expression_t<Parser>;
*/
		}
		// statement { variable_definition }
		namespace
		{
			namespace
			{
				template<typename Parser, typename = void>
				struct variable_definition
				{
					using type = parser_error_expected<Parser, token::decorator::token_kind::identifier>;
				};
				
				template<typename Parser>
				struct variable_definition<Parser,
					std::enable_if_t<(Parser::token::kind == token::decorator::token_kind::identifier)>>
				{
					using identifier = typename node::template identifier<typename Parser::token>;
					using next_state_0 = typename Parser::pop_front;
					using next_state_1 = typename std::conditional_t<
						(next_state_0::token::kind == token::decorator::token_kind::operator_assign),
						typename next_state_0::pop_front,
						parser_error_expected<next_state_0, token::decorator::token_kind::operator_assign>>;
					using operand = make_expression_t<next_state_1>;
					using next_state = typename operand::next_state;
					using node = typename node::template variable_definition<identifier, typename operand::node>;
				};
				
				template<typename Parser>
				using make_variable_definition_t = variable_definition<Parser>;
			}

			template<typename Parser, typename = void>
			struct statement
			{
				using type = parser_error_unexpected<Parser, Parser::token::kind>;
			};

			template<typename Parser>
			struct statement<Parser,
				std::enable_if_t<(Parser::token::kind == token::decorator::token_kind::keyword_let)>>
			{
				using next_state = typename Parser::pop_front;
				using type = make_variable_definition_t<next_state>;
			};

			template<typename Parser>
			using make_statement_t = typename statement<Parser>::type;
		}

		// program
		namespace
		{
			template<typename Parser, typename Node, typename = void>
			struct program
			{
				using statement = make_statement_t<Parser>;
				using next_state = typename statement::next_state;
				using node = typename Node::template add_statement<typename statement::node>;
				using type = typename program<next_state, node>::type;
			};

			template<typename Parser, typename Node>
			struct program<Parser, Node,
				std::enable_if_t<(Parser::token::kind == token::decorator::token_kind::end_of_source)>>
			{
				using next_state = Parser;
				using node = Node;
				using type = program<Parser, Node>;
			};

			template<typename Parser>
			using make_program_t = typename program<Parser, typename node::template program<type_list<>>>::type;
		}
	}

	template<typename Tokens>
	struct parser
	{
		using type = parser<Tokens>;
		using tokens = Tokens;
		using token = typename Tokens::template get<0>;

		using advance = std::conditional_t<(token::kind == token::decorator::token_kind::end_of_source), type,
			typename parse_rules::template make_program_t<type>>;

		using pop_front = parser<typename Tokens::template pop_front<1>>;
	};
}
