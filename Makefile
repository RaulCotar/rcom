CFLAGS := $(shell grep '^[^#]' compile_flags.txt)
HEADERS := $(wildcard src/*.h)
TESTS := $(patsubst test/%.c,bin/%.test,$(wildcard test/*.c))

INST_PREFIX ?= /usr/local
HEADER_DIR := $(INST_PREFIX)/include/rcom
LIB_DIR := $(INST_PREFIX)/lib

INSTALL_TGTS := $(patsubst src/%.h,$(HEADER_DIR)/%.h,$(HEADERS)) $(INST_PREFIX)/lib/pkgconfig/rcom.pc

.ONESHELL:
.PHONY: help test install uninstall list clean

help:
	@echo 'RCOM make options:'
	@echo -e '\e[1;37mTargets:\e[0m'
	@echo -e '  \e[32mhelp\t\t\e[37mdisplay this help text\e[0m'
	@echo -e '  \e[32mtest\t\t\e[37mcompile and run the test files\e[0m'
	@echo -e '  \e[32minstall\t\e[37minstall the header files\e[0m'
	@echo -e '  \e[32muninstall\t\e[37muninstall all the related files\e[0m'
	@echo -e '  \e[32mlist\t\t\e[37mlist all files to be installed\e[0m'
	@echo -e '  \e[32mclean\t\t\e[37mremove the ./bin directory\e[0m'
	@echo -e '\e[1;37mVariables:\e[0m'
	@echo -e '  \e[36mINST_PREFIX\t\e[37minstall dir prefix (current: "$(INST_PREFIX)")\e[0m'
	@echo -e '\e[1;37mEnv vars:\e[0m'
	@echo -e "  \e[35mCC\t\t\e[37mC compiler (current: \"$$CC\")\e[0m"

test: $(TESTS)

install: $(INSTALL_TGTS)

uninstall:
	sudo rm -rfv $(INST_PREFIX)/include/rcom $(INST_PREFIX)/lib/pkgconfig/rcom.pc

list:
	@echo $(INSTALL_TGTS)

clean:
	rm -rf bin

bin/%.test: test/%.c | bin
	$$CC $$(grep '^[^#]' compile_flags.txt) $^ -o $@ && ./$@

$(INST_PREFIX)/include/rcom/%.h: src/%.h | $(INST_PREFIX)/include/rcom
	sudo cp $< $@

$(INST_PREFIX)/lib/pkgconfig/rcom.pc: src/rcom.pc | $(INST_PREFIX)/lib/pkgconfig
	sed 's/INST_PREFIX/$(subst /,\/,$(INST_PREFIX))/' src/rcom.pc > bin/rcom.pc.temp && sudo mv bin/rcom.pc.temp $@

$(INST_PREFIX)/include/rcom $(INST_PREFIX)/lib/pkgconfig:
	sudo mkdir -p $@

bin:
	mkdir $@