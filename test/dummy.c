// clang -std=gnu23 -Wall -Wextra -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address -Og -ggdb3 -lrcom -Wl,-rpath=/usr/local/lib
#include <stdio.h>
#include <rcom/foo.h>

int main() {
	printf("Hello dummy %d!", foo());
	return 0;
}
