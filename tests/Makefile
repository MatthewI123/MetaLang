include $(TOOLS)/generate_tokens/Makefile.variable

INCLUDES := $(wildcard $(INCLUDE)/metalang/*.hpp) $(wildcard $(INCLUDE)/metalang/compiler/*.hpp) $(wildcard $(INCLUDE)/metalang/compiler/parser_rules/*.hpp)
GENERATE_TOKENS_INCLUDES := $(wildcard $(TOOLS)/generate_tokens/*.hpp)

.PHONY: all clean

all: $(DEPENDENCIES) tests.out

clean:
	rm -f tests.out $(DEPENDENCIES)

$(DEPENDENCIES): source.metalang $(GENERATE_TOKENS_INCLUDES)
	# ====== BUILD: dependencies ====== #
	$(TOOLS)/generate_tokens/generate_tokens.out source.metalang

tests.out: main.cpp $(DEPENDENCIES) $(INCLUDES)
	$(CXX) $< $(SOURCE_DEPENDENCIES) -o $@
