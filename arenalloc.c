#include "arenalloc.h"
#include "builtins.h"
#include "mman.h"
#include "debug.h"
#include <stdio.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096 // TODO: un-hardcode

arenalloc_t arena_new(u64 size, enum arenalloc_flags flags) {
	arenalloc_t arena = {NULL, NULL, NULL};
	if (!size)
		return arena;
	if (flags & ARENALLOC_USE_ALLOCA)
		arena.base = __builtin_alloca(size);
	else {
		size = ((size-1)/PAGE_SIZE+1)*PAGE_SIZE; // valid for size != 0
		arena.base = mmap(NULL, size, PROT_RW, MAP_ANONPRIV, -1, 0);
	}
	if (likely(arena.base)) {
		arena.end = arena.base + size;
		arena.head = arena.base;
	}
	return arena;
}

int arena_resize(arenalloc_t *arena, u64 size, enum arenalloc_flags flags) {
	if (!size)
		return arena_release(arena);
	if (!(arena->end - arena->base)) {
		*arena = arena_new(size, false);
		return arena->end - arena->end;
	}
	if (!(flags & ARENALLOC_MAY_RESIZE_USED))
		size = MAX((i64)size, arena->head - arena->base);
	size = ((size-1)/PAGE_SIZE+1)*PAGE_SIZE; // valid for size != 0
	void *tmp = mremap(arena->base, arena->end - arena->base, size, flags & ARENALLOC_MAY_MOVE ? MREMAP_MAYMOVE : 0);
	if (tmp == MAP_FAILED)
		return -1;
	arena->end = tmp + size;
	arena->head = tmp + (arena->head - arena->base);
	arena->base = tmp;
	return 0;
}

int arena_release(arenalloc_t *arena) {
	if (arena->end - arena->base) {
		int const x = munmap(arena->base, arena->end - arena->base);
		if (x) return x;
	}
	arena->base = (arena->end = (arena->head = NULL));
	return 0;
}

void *arena_alloc(arenalloc_t *arena, u64 size, enum arenalloc_flags flags) {
	assert(arena->end - arena->base);
	if (!size)
		return NULL;
	if ((u64)(arena->end - arena->head) < size)
		if (arena_resize(arena, arena->head - arena->base + size, flags & ARENALLOC_MAY_MOVE))
			return NULL;
	arena->head += size;
	return arena->head - size;
}

void *arena_free(arenalloc_t *arena, u64 size) {
	if (!(arena->end - arena->base))
		return NULL;
	if ((i64)size >= arena->head - arena->base)
		arena->head = arena->base;
	else
		arena->head -= size;
	return arena->head;
}

#ifndef NDEBUG
void arena_print(arenalloc_t arena, char const* msg) {
	fprintf(stderr, "\e[33m%s: arena [%p -> %p @ %p] (total: %ld, used: %ld, free: %ld)\e[39m\n",
		msg, arena.base, arena.end, arena.head, arena.end-arena.base, arena.head-arena.base, arena.end-arena.head);
	fflush(stderr);
}
#endif
