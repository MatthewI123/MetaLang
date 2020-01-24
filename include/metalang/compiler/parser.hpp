#pragma once

#include <type_traits>
#include "parser_rules/program.hpp"
#include "error.hpp"
#include "node.hpp"
#include "token.hpp"

namespace compiler
{
	template<typename Tokens>
	struct parser
	{
		using parser_type = parser<Tokens>;
		using tokens = Tokens;

		using current = typename Tokens::template get<0>;

		template<token::decorator::token_kind Kind>
		static constexpr bool test = current::kind == Kind;

		static constexpr bool done = test<token::decorator::token_kind::end_of_source>;

		using next = std::conditional_t<done, parser_type, parser<typename Tokens::template pop_front<1>>>;

		template<token::decorator::token_kind Kind>
		using expect = std::conditional_t<test<Kind>, next, parser_error_expected<parser_type, Kind>>;

		template<token::decorator::token_kind Kind = current::kind>
		using unexpected = parser_error_unexpected<parser_type, Kind>;
	};
}
