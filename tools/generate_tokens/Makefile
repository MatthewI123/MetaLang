include Makefile.variable

.PHONY: all clean

all: generate_tokens.out

clean:
	rm -f generate_tokens.out $(DEPENDENCIES)

generate_tokens.out: main.cpp source.hpp target.hpp $(INCLUDE)/metalang/types.hpp
	$(CXX) $< -o $@
