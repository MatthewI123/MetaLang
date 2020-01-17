.PHONY: all clean

all:
	gcc --std=c++17 -Iinclude/ -Wall -Wextra -Wpedantic tests/test.cpp -o test.out

clean:
	rm -f test.out
