#pragma once

#include <type_traits>
#include "variable_definition.hpp"
#include "function_definition.hpp"
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	// let <identifier> = <expression>
	//                  ^
	template<typename Parser, typename Identifier, typename = void>
	class make_statement
	{
	private:
		using next_state_0 = typename Parser::template expect<token::decorator::token_kind::operator_assign>;

	public:
		using type = make_variable_definition<next_state_0, Identifier>;
	};

	// let <identifier>([<identifier_list>]) = <expression>
	//                 ^
	template<typename Parser, typename Identifier>
	class make_statement<Parser, Identifier,
		std::enable_if_t<Parser::current::kind == token::decorator::token_kind::symbol_left_parenthesis>>
	{
	public:
		using type = make_function_definition<typename Parser::next, Identifier>;
	};

	// let <identifier> *
	// ^
	template<typename Parser>
	class make_statement_dispatch
	{
	private:
		using next_state_0 = typename Parser::template expect<token::decorator::token_kind::keyword_let>;
		using next_state_1 = typename next_state_0::template expect<token::decorator::token_kind::identifier>;
		using identifier = typename node::template identifier<typename next_state_0::current>;
	
	public:
		using type = typename make_statement<next_state_1, identifier>::type;
	};

	template<typename Parser>
	using make_statement_t = typename make_statement_dispatch<Parser>::type;
}
