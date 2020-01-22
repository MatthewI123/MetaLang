#include <iostream>
#include "source.hpp"
#include "target.hpp"
#include "types.hpp"

int main()
{
	// current directory: build/
	source input("../source.metalang");
	target output("source.metalang");

	while (true) {
		auto [position, line, column] = input.here();
		char current = input.get();

		if (std::isalpha(current) || current == '_') { // identifier
			std::string lexeme;

			while (std::isalpha(current) || std::isdigit(current) || current == '_') {
				lexeme += current;
				current = input.get();
			}

			if (lexeme == "let")
				output.add_symbol("keyword_let", position, line, column);
			else
				output.add_identifier(lexeme, position, line, column);
		} else if (std::isdigit(current)) { // integer
			std::string lexeme;
			unsigned long long value;

			while (std::isdigit(current)) {
				lexeme += current;
				current = input.get();
			}

			try {
				value = std::stoull(lexeme);

				if (value > std::numeric_limits<word_t>::max())
					std::cout << "[warning]: integer " << lexeme << " will be truncated.\n";

				output.add_integer(value, position, line, column);
			} catch (...) {
				input.error("unknown error trying to parse integer", position, line, column);
			}
		} else if (current == ' ' || current == '\n') {
			continue;
		} else if (current != std::ifstream::traits_type::eof()) {
			std::string kind;

			switch (current) {
				case '=': kind = "operator_assign"; break;
				case '+': kind = "operator_add"; break;
				case '-': kind = "operator_minus"; break;
				case '*': kind = "operator_multiply"; break;
				case '/': kind = "operator_divide"; break;
				case '%': kind = "operator_modulo"; break;
				case '(': kind = "symbol_left_parenthesis"; break;
				case ')': kind = "symbol_right_parenthesis"; break;
				default:
					input.error(std::string("unexpected symbol ") + current, position, line, column);
					break;
			}

			output.add_symbol(kind, position, line, column);
		} else {
			break;
		}
	}
}
