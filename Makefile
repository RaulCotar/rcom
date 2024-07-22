MAKEFLAGS += --no-builtin-rules --no-builtin-variables
C_FILES := $(wildcard *.c)
H_FILES := $(wildcard *.h)
TESTS := $(patsubst test/%.c,build/%.log,$(wildcard test/*.c))
LIB_DIR := /usr/local/lib
H_DIR := /usr/local/include/rcom
VERSION := $(file < version.txt)
VMAJOR := $(firstword $(subst ., ,$(VERSION)))
CC ?= clang
CFLAGS := $(shell grep '^[^#]' compile_flags.txt) -U_RCOM_VERSION -D_RCOM_VERSION=\"$(VERSION)\"

.PHONY: all install heads libs test
all: install test
install: heads libs
heads: $(patsubst %,$(H_DIR)/%,$(H_FILES))
libs: $(LIB_DIR)/librcom.a $(LIB_DIR)/librcom.so
test: $(TESTS)

$(H_DIR)/%.h: %.h | $(H_DIR)
	sudo cp $< $@

$(H_DIR):
	sudo mkdir -v $@

$(LIB_DIR)/%: build/%
	sudo cp $< $@

build/librcom.so: $(C_FILES) $(H_FILES) compile_flags.txt version.txt | build
	$(CC) $(CFLAGS) -fpic -shared -Wl,-soname,librcom.so $(C_FILES) -o $@

build/librcom.a: $(patsubst %.c,build/%.o,$(C_FILES)) | build
	ar rcs $@ $^

build/%.o: %.c $(H_FILES) compile_flags.txt version.txt | build
	$(CC) $(CFLAGS) -c $(C_FILES) -o $@

build/%.log: build/%.test
	(./$< > $@) || (echo -e "\e[31m$< failed with exit code $$?\e[39m" && false)

.SECONDARY: $(TESTS:.log=.test)
build/%.test: test/%.c $(patsubst %,$(H_DIR)/%,$(H_FILES)) $(LIB_DIR)/librcom.a $(LIB_DIR)/librcom.so | build
	$(shell head -n1 $< | tail -c+3) $< -o $@

build:
	mkdir $@

.PHONY: clean
clean:
	rm -rf build
	sudo rm -rfIv $(LIB_DIR)/librcom.* $(H_DIR)

.PHONY: help
help:
	@echo -e '\e[1mRcom $(VERSION)\e[22m'
	@echo "Make targets and variables:"
	@echo -e "all\t\tbuild, install, and run tests (default target)"
	@echo -e "install\t\tbuild and copy everything to target directories (libs and headers)"
	@echo -e "heads\t\tcopy the header files to their target directory"
	@echo -e "libs\t\tbuild and copy the library files to their target directory"
	@echo -e "test\t\tbuild and run the tests"
	@echo -e "clean\t\tremove headers, libs and the build directory"
	@echo -e "\$$H_DIR\t\theader install directory"
	@echo -e "\$$LIB_DIR\tlibrary install directory"
