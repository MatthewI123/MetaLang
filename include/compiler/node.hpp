#pragma once

#include "types.hpp"

namespace compiler::node
{
	namespace decorator
	{
		enum class node_kind
		{
			additive_expression, multiplicative_expression, unary_expression,
			identifier, integer
		};

		enum class additive_mode { addition, subtraction };
		enum class multiplicative_mode { multiplication, division, modulo };
		enum class unary_mode { negation };
	};

	template<decorator::additive_mode Mode, typename Left, typename Right>
	struct additive_expression
	{
		static constexpr auto kind = decorator::node_kind::additive_expression;
		using mode = Mode;
		using left = Left;
		using right = Right;
	};

	template<decorator::multiplicative_mode Mode, typename Left, typename Right>
	struct multiplicative_expression
	{
		static constexpr auto kind = decorator::node_kind::multiplicative_expression;
		using mode = Mode;
		using left = Left;
		using right = Right;
	};

	template<decorator::unary_mode Mode, typename Operand>
	struct unary_expression
	{
		static constexpr auto kind = decorator::node_kind::unary_expression;
		using mode = Mode;
		using operand = Operand;
	};

	template<const char* const Value, typename Token>
	struct identifier
	{
		using token = Token;
		static constexpr auto kind = decorator::node_kind::identifier;
		static constexpr auto value = Value;
	};

	template<word_t Value, typename Token>
	struct integer
	{
		using token = Token;
		static constexpr auto kind = decorator::node_kind::integer;
		static constexpr auto value = Value;
	};
}
