#pragma once

#include "token.hpp"

namespace compiler
{

	template<typename Parser, token::decorator::token_kind Kind>
	struct parser_error_expected
	{
		using parser = Parser;
		static constexpr auto kind = Kind;
		static constexpr auto expected = Kind;
		static constexpr auto got = Parser::current::kind;

		static_assert(Kind != Kind, "parser error: expected <expected>, got <got>");
	};
}
