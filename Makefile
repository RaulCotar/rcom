MAKEFLAGS += --no-builtin-rules --no-builtin-variables
C_FILES := $(wildcard *.c)
H_FILES := $(wildcard *.h)
LIB_DIR := /usr/lib/rcom
CC ?= clang
CFLAGS := $(shell grep '^[^#]' compile_flags.txt) -UVERSION -DVERSION="$(file < version.txt)"

.PHONY: all
all: $(LIB_DIR) $(patsubst %,$(LIB_DIR)/%,$(H_FILES)) $(LIB_DIR)/librcom.a $(LIB_DIR)/librcom.so

$(LIB_DIR):
	sudo mkdir -v $@

$(LIB_DIR)/%.h: %.h
	cp $< $@

$(LIB_DIR)/librcom.so: $(C_FILES) $(H_FILES) compile_flags.txt version.txt
	$(CC) $(CFLAGS) -fpic -shared $(C_FILES) -o $@

$(LIB_DIR)/librcom.a: $(patsubst %.c,%.o,$(C_FILES))
	ar rcs $@ $^

%.o: %.c $(H_FILES) compile_flags.txt version.txt
	$(CC) $(CFLAGS) -c $(C_FILES) -o $@

.PHONY: clean
clean:
	rm -f *.o $(LIB_DIR)/*
