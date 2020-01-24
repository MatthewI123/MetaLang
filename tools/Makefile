.PHONY: all clean
.PHONY: generate_tokens clean_generate_tokens

all: generate_tokens

clean: clean_generate_tokens

generate_tokens:
	# === BUILD: generate_tokens === #
	$(MAKE) -C generate_tokens/ all

clean_generate_tokens:
	# === CLEAN: generate_tokens === #
	$(MAKE) -C generate_tokens/ clean
