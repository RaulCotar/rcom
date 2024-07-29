// clang -std=gnu23 -Wall -Wextra -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address -Og -ggdb3 -lrcom -Wl,-rpath=/usr/local/lib
#include <rcom/arenalloc.h>
#include <rcom/builtins.h>
#include <rcom/debug.h>

int main() {
	char const HELLO[] = "Hello!";

	arenalloc_t arena = arena_new(123, false);

	char *hello = arena_alloc(&arena, 7, false);
	strcpy(hello, HELLO);
	assert(!strcmp(hello, HELLO));

	hello = arena_alloc(&arena, 5000, true);
	strncpy(hello + 5000 - 7, HELLO, 7);
	strncpy(hello, HELLO, 7);

	arena_free(&arena, 5000);
	assert(!strcmp(hello, HELLO));

	arena_free(&arena, 1);
	assert(!strncmp(hello, HELLO, 6));

	arena_free(&arena, 1);
	arena_resize(&arena, 1, true);

	arena_release(&arena);
	return 0;
}
