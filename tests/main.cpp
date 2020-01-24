#include <typeinfo>
#include <metalang/compiler/parser.hpp>
#include "generated_tokens.hpp"

int main()
{
	using parser = typename compiler::parser<typename generated::tokens>;
	using tree = typename compiler::parser_rules::template make_program_t<parser>;

	return 0;
}
