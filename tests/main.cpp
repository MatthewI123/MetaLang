#include <iostream>
#include <string>
#include <typeinfo>
#include <utility>
#include <metalang/compiler/parser.hpp>
#include "generated_tokens.hpp"

using namespace compiler;

template<typename Node, std::size_t... Index>
void traverse_program(std::size_t depth, std::index_sequence<Index...>);

template<typename Node>
void traverse(std::size_t depth)
{
	std::string prefix(depth, ' ');

	if constexpr (Node::kind == node::decorator::node_kind::program) {
		std::cout << prefix << "program" << '\n';

		traverse_program<Node>(depth + 1, std::make_index_sequence<Node::statements::size>());
	} else if constexpr (Node::kind == node::decorator::node_kind::variable_definition) {
		std::cout << prefix << "variable_definition\n";

		traverse<typename Node::identifier>(depth + 1);
		traverse<typename Node::operand>(depth + 1);
	} else if constexpr (Node::kind == node::decorator::node_kind::additive_expression) {
		std::cout << prefix << "additive_expression ";
		
		if constexpr (Node::mode == node::decorator::additive_mode::addition)
			std::cout << "(addition)\n";
		else if constexpr (Node::mode == node::decorator::additive_mode::subtraction)
			std::cout << "(subtraction)\n";
		else
			std::cout << "(<invalid>)\n";
		
		traverse<typename Node::left>(depth + 1);
		traverse<typename Node::right>(depth + 1);
	} else if constexpr (Node::kind == node::decorator::node_kind::multiplicative_expression) {
		std::cout << prefix << "multiplicative_expression ";
		
		if constexpr (Node::mode == node::decorator::multiplicative_mode::multiply)
			std::cout << "(multiply)\n";
		else if constexpr (Node::mode == node::decorator::multiplicative_mode::divide)
			std::cout << "(divide)\n";
		else if constexpr (Node::mode == node::decorator::multiplicative_mode::modulo)
			std::cout << "(modulo)\n";
		else
			std::cout << "(<invalid>)\n";
		
		traverse<typename Node::left>(depth + 1);
		traverse<typename Node::right>(depth + 1);
	} else if constexpr (Node::kind == node::decorator::node_kind::unary_expression) {
		std::cout << prefix << "unary_expression ";

		if constexpr(Node::mode == node::decorator::unary_mode::negation)
			std::cout << "(negation)\n";
		else
			std::cout << "(<invalid>)\n";
		
		traverse<typename Node::operand>(depth + 1);
	} else if constexpr (Node::kind == node::decorator::node_kind::identifier) {
		std::cout << prefix << "identifier = " << Node::value << '\n';
	} else if constexpr (Node::kind == node::decorator::node_kind::integer) {
		std::cout << prefix << "integer = " << Node::value << '\n';
	} else {
		std::cout << "<invalid> (kind = " << static_cast<int>(Node::kind) << ")\n";
	}
}

template<typename Node, std::size_t... Index>
void traverse_program(std::size_t depth, std::index_sequence<Index...>)
{
	(... , traverse<typename Node::statements::template get<Index>>(depth));
}

int main()
{
	using parser = parser<typename generated::tokens>;
	using tree = typename parser_rules::template make_program_t<parser>;
	traverse<typename tree::node>(0);

	return 0;
}
