.PHONY: all clean tokens

CXX := g++ --std=c++17 -Iinclude/ -Wall -Wextra -Wpedantic -O3

all:
	$(CXX) tests/test.cpp -o test.out

tokens: build/tokens.hpp
	# build/tokens.hpp
	# build/lexemes.cpp
	# build/lexemes.hpp

build/tokens.hpp: src/generate_tokens/main.cpp src/generate_tokens/source.hpp src/generate_tokens/target.hpp source.metalang
	mkdir -p build/
	$(CXX) $< -o build/generate_tokens.out
	cd build && ./generate_tokens.out

clean:
	rm -f build/*
	rm -f test.out
