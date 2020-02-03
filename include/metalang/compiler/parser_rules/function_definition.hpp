#pragma once

#include <type_traits>
#include "expression.hpp"
#include "../../types.hpp"
#include "../node.hpp"
#include "../token.hpp"

namespace compiler::parser_rules
{
	// let <identifier>([<identifier_list>]) = <expression>
	//                  ^
	template<typename Parser, typename Identifier>
	class make_function_definition
	{
	private:
		// let <identifier>([<identifier_list>]) = <expression>
		//                                     ^
		template<typename IntermediateParser, typename Parameters>
		class make_parameters
		{
		public:
			using next_state = typename IntermediateParser::template expect<
				token::decorator::token_kind::symbol_right_parenthesis>;
			using list = Parameters;
		};

		// let <identifier>([<identifier_list>], <identifier>) = <expression>
		//                                     ^
		template<typename IntermediateParser, typename Parameters, typename = void>
		class make_parameters_helper
		{
		private:
			using next_state_0 = typename IntermediateParser::template expect<
				token::decorator::token_kind::symbol_delimiter>;
			using parameter = typename node::template identifier<typename next_state_0::current>;
		
		public:
			using type = typename make_parameters_helper<typename next_state_0::next,
				typename Parameters::template push_back<parameter>>::type;
		};

		// let <identifier>([<identifier_list>]) = <expression>
		//                                     ^
		template<typename IntermediateParser, typename Parameters>
		class make_parameters_helper<IntermediateParser, Parameters, std::enable_if_t<
				IntermediateParser::current::kind == token::decorator::token_kind::symbol_right_parenthesis>>
		{
		public:
			using type = make_parameters<IntermediateParser, Parameters>;
		};

		// let <identifier>(<identifier_list>) = <expression>
		//                  ^
		template<typename IntermediateParser, typename = void>
		class make_parameters_dispatch
		{
		private:
			using next_state_0 = typename IntermediateParser::template expect<token::decorator::token_kind::identifier>;

		public:
			using identifier = typename node::template identifier<typename IntermediateParser::current>;
			using type = typename make_parameters_helper<next_state_0, type_list<identifier>>::type;
		};

		// let <identifier>() = <expression>
		//                  ^
		template<typename IntermediateParser>
		class make_parameters_dispatch<IntermediateParser, std::enable_if_t<
			IntermediateParser::current::kind == token::decorator::token_kind::symbol_right_parenthesis>>
		{
		public:
			using type = make_parameters<Parser, type_list<>>;
		};

		using parameters = typename make_parameters_dispatch<Parser>::type;
		using next_state_0 = typename parameters::next_state::template expect<
			token::decorator::token_kind::operator_assign>;
		using operand = make_expression_t<next_state_0>;

	public:
		using next_state = typename operand::next_state;
		using node = typename node::template function_definition<Identifier, typename parameters::list,
			typename operand::node>;
	};

	template<typename Parser, typename Identifier>
	using make_function_definition_t = make_function_definition<Parser, Identifier>;
}
