.PHONY: all clean
.PHONY: tools clean_tools
.PHONY: tests clean_tests

export INCLUDE := $(CURDIR)/include
export TOOLS := $(CURDIR)/tools
export CXX := g++ --std=c++17 -I$(INCLUDE)/ -Wall -Wextra -Wpedantic -Wfatal-errors -O3

all: tools tests

clean: clean_tools clean_tests

tools:
	# === BUILD: tools === #
	$(MAKE) -C tools/ all

clean_tools:
	# === CLEAN: tools === #
	$(MAKE) -C tools/ clean

tests: tools
	# === BUILD: tests === #
	$(MAKE) -C tests/ all

clean_tests:
	# === CLEAN: tests === #
	$(MAKE) -C tests/ clean
