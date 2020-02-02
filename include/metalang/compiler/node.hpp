#pragma once

namespace compiler::node
{
	namespace decorator
	{
		enum class node_kind
		{
			program, variable_definition, function_definition,
			additive_expression, multiplicative_expression, unary_expression,
			identifier, integer
		};

		enum class additive_mode { addition, subtraction };
		enum class multiplicative_mode { multiply, divide, modulo };
		enum class unary_mode { negation };
	}

	template<typename Statements>
	struct program
	{
		static constexpr auto kind = decorator::node_kind::program;
		using statements = Statements;

		template<typename Statement>
		using add_statement = program<typename Statements::template push_back<Statement>>;
	};

	template<typename Identifier, typename Operand>
	struct variable_definition
	{
		static constexpr auto kind = decorator::node_kind::variable_definition;
		using identifier = Identifier;
		using operand = Operand;
	};

	template<typename Identifier, typename Parameters, typename Operand>
	struct function_definition
	{
		static constexpr auto kind = decorator::node_kind::function_definition;
		using identifier = Identifier;
		using parameters = Parameters;
		using operand = Operand;

		template<typename Parameter>
		using add_parameter = function_definition<Identifier, typename Parameters::template push_back<Parameter>,
			Operand>;
	};

	template<decorator::additive_mode Mode, typename Left, typename Right>
	struct additive_expression
	{
		static constexpr auto kind = decorator::node_kind::additive_expression;
		static constexpr auto mode = Mode;
		using left = Left;
		using right = Right;
	};

	template<decorator::multiplicative_mode Mode, typename Left, typename Right>
	struct multiplicative_expression
	{
		static constexpr auto kind = decorator::node_kind::multiplicative_expression;
		static constexpr auto mode = Mode;
		using left = Left;
		using right = Right;
	};

	template<decorator::unary_mode Mode, typename Operand>
	struct unary_expression
	{
		static constexpr auto kind = decorator::node_kind::unary_expression;
		static constexpr auto mode = Mode;
		using operand = Operand;
	};

	template<typename Token>
	struct identifier
	{
		using token = Token;
		static constexpr auto kind = decorator::node_kind::identifier;
		static constexpr auto value = Token::value;
	};

	template<typename Token>
	struct integer
	{
		using token = Token;
		static constexpr auto kind = decorator::node_kind::integer;
		static constexpr auto value = Token::value;
	};
}
