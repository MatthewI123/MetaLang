#include <iostream>
#include <metalang/types.hpp>
#include "source.hpp"
#include "target.hpp"

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "usage: generate_tokens <file>\n";
		std::cout << "  Generates tokens.hpp, lexemes.hpp, lexemes.cpp in current directory.\n";
		return 1;
	}

	source input(argv[1]);
	target output(argv[1]);

	while (true) {
		auto [position, line, column] = input.here();
		char current = input.peek();

		if (std::isalpha(current) || current == '_') { // identifier
			std::string lexeme;

			while (std::isalpha(input.peek()) || std::isdigit(input.peek()) || input.peek() == '_')
				lexeme += input.get();

			if (lexeme == "let")
				output.add_symbol("keyword_let", position, line, column);
			else
				output.add_identifier(lexeme, position, line, column);
		} else if (std::isdigit(current)) { // integer
			std::string lexeme;
			unsigned long long value;

			while (std::isdigit(input.peek()))
				lexeme += input.get();

			try {
				value = std::stoull(lexeme);

				if (value > std::numeric_limits<word_t>::max())
					std::cout << "[warning]: integer " << lexeme << " will be truncated.\n";

				output.add_integer(value, position, line, column);
			} catch (...) {
				input.error("unknown error trying to parse integer", position, line, column);
			}
		} else if (current == ' ' || current == '\n') {
			input.get();
			continue;
		} else if (current != std::ifstream::traits_type::eof()) {
			std::string kind;

			switch (current) {
				case '=': kind = "operator_assign"; break;
				case '+': kind = "operator_add"; break;
				case '-': kind = "operator_subtract"; break;
				case '*': kind = "operator_multiply"; break;
				case '/': kind = "operator_divide"; break;
				case '%': kind = "operator_modulo"; break;
				case ',': kind = "symbol_delimiter"; break;
				case '(': kind = "symbol_left_parenthesis"; break;
				case ')': kind = "symbol_right_parenthesis"; break;
				default:
					input.error(std::string("unexpected symbol ") + current, position, line, column);
					break;
			}

			input.get();
			output.add_symbol(kind, position, line, column);
		} else {
			break;
		}
	}
}
