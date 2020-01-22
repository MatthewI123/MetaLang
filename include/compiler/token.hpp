#pragma once

#include "types.hpp"

namespace compiler::token
{
	using void_t = std::nullptr_t;
	using word_t = ::word_t;
	using lexeme_t = const char*;

	namespace decorator
	{
		enum class token_kind
		{
			keyword_let,
			operator_assign,
			operator_add, operator_minus,
			operator_multiply, operator_divide, operator_modulo,
			symbol_left_parenthesis, symbol_right_parenthesis,
			identifier, integer, end_of_source
		};
	}

	template<decorator::token_kind Kind, size_t Position, size_t Line, size_t Column,
		typename LiteralType, LiteralType Value>
	struct token
	{
		using literal_type = LiteralType;

		static constexpr auto kind = Kind;
		static constexpr auto position = Position;
		static constexpr auto line = Line;
		static constexpr auto column = Column;
		static constexpr auto value = Value;
	};
	
	template<decorator::token_kind, size_t, size_t, size_t, typename LiteralType, LiteralType>
	struct make_token;

	// handle simple tokens
	template<decorator::token_kind Symbol, size_t Position, size_t Line, size_t Column>
	struct make_token<Symbol, Position, Line, Column, void_t, void_t{}>
	{
		using type = token<Symbol, Position, Line, Column, void_t, void_t{}>;
	};

	// handle identifier
	template<size_t Position, size_t Line, size_t Column, lexeme_t Lexeme>
	struct make_token<decorator::token_kind::identifier, Position, Line, Column, lexeme_t, Lexeme>
	{
		using type = token<decorator::token_kind::identifier, Position, Line, Column, lexeme_t, Lexeme>;
	};

	// handle integer
	template<size_t Position, size_t Line, size_t Column, word_t Integer>
	struct make_token<decorator::token_kind::integer, Position, Line, Column, word_t, Integer>
	{
		using type = token<decorator::token_kind::identifier, Position, Line, Column, word_t, Integer>;
	};

	template<decorator::token_kind Kind, size_t Position, size_t Line, size_t Column, auto Literal = void_t()>
	using make_token_t = typename make_token<Kind, Position, Line, Column, decltype(Literal), Literal>::type;
}
