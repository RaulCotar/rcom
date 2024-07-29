// clang -std=gnu23 -Wall -Wextra -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address -Og -ggdb3 -lrcom -Wl,-rpath=/usr/local/lib
#include <rcom/arenalloc.h>
#include <rcom/builtins.h>
#include <rcom/debug.h>

unsigned char rev(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int main() {
	char const HELLO[] = "Hello!";

	arenalloc_t arena = arena_new(123, true);
	// arena_print(arena, "arena_new");

	char *hello = arena_alloc(&arena, 7, false);
	// fprintf(stderr, "> %p\n", hello);
	// arena_print(arena, "arena_alloc");
	for (u32 i=0; i<7; i++)
		hello[i] = HELLO[i];
	// strcpy(hello, HELLO);
	// puts(hello);
	for (u32 i=0; i<7; i++)
		assert(*(hello+i) == HELLO[i]);
	// putchar('\n');

	hello = alloca(8);
	for (u32 i=0; i<7; i++)
		hello[i] = HELLO[i];
	// puts(hello);
	for (u32 i=0; i<7; i++)
		assert(*(hello+i) == HELLO[i]);
	// putchar('\n');

	arena_release(&arena);
	// arena_print(arena, "arena_release");
	return 0;
}
