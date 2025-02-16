#ifndef _ARENALLOC_H_
#define _ARENALLOC_H_

#include <unistd.h> // mremap, getpagesize
#ifndef NDEBUD // arenaDebugPrint needs these
	#include <stdio.h>
	#include <string.h>
#endif
#include "mman.h"
#include "types.h"

typedef struct arena_t {
	void *base; // NULL means uninitialized
	void *head; // ptr to first free byte
	void *top; // non-inclusive
} arena_t;

#define EMPTY_ARENA ((arena_t){NULL, NULL, NULL})

/// Allocate `size` bytes from the arena.
void *arenaAlloc(arena_t *arena, u64 size);

/// Allocate an aligned block of memory from the arena.
void *arenaAllocAlign(arena_t *arena, u64 size, i64 align, i64 offset);

/// Allocate and try to grow by a factor of 2 if resizing is needed.
void *arenaAllocGrow(arena_t *arena, u64 size, bool mayMove);

/// Free (at most) the top `size` bytes allcated from the arena.
u64 arenaFree(arena_t *arena, u64 size);

/// Ensure there are at least `size` free bytes in the arena.
bool arenaReserve(arena_t *arena, u64 size, bool mayGrow, bool mayMove);

/// Remap and resize the arena.
bool arenaRemap(arena_t *arena, u64 size, bool mayMove);

/// Reset the allocation head pointer back to the arena base.
void arenaReset(arena_t *arena);

/// Return all of the arena's memory to the OS.
bool arenaUnmap(arena_t *arena);

/// Get the total number of mmaped bytes in the arena (aka capacity).
u64 arenaGetMappedSize(arena_t const *arena);

/// Get the amount of free bytes left in the arena.
u64 arenaGetFreeSize(arena_t const *arena);

/// Get the amount of used bytes in the arena.
u64 arenaGetUsedSize(arena_t const *arena);

/// Update the values in `_arenaPageSizes`.
void arenaUpdatePageSizes();

#ifndef NDEBUG
/// Print the arena. (uses ANSI escape sequnces)
void arenaDebugPrint(arena_t const *arena);
#endif // ifndef NDEBUG

#define ARENALLOC_IMPL
#ifdef ARENALLOC_IMPL

u64 const _arenaPageSizes[] = {4096};

void *arenaAlloc(arena_t *arena, u64 size) {
	if (size > (u64)(arena->top - arena->head))
		return NULL;
	void *const ret = arena->head;
	arena->head += size;
	return ret;
}

void *arenaAllocAlign(arena_t *arena, u64 size, i64 align, i64 offset) {
	i64 const rest = (i64)arena->head % align;

	void *start = arena->head + offset - rest;
	if (rest > offset)
		start += align;

	if (size > (u64)(arena->top - start))
		return NULL;
	arena->head = start + size;
	return start;
}

void *arenaAllocGrow(arena_t *arena, u64 size, bool mayMove) {
	if (!arenaReserve(arena, size, false, false))
		if (!arenaRemap(arena, (arena->top - arena->base) * 2, mayMove))
			return NULL;
	return arenaAlloc(arena, size);
}

u64 arenaFree(arena_t *arena, u64 size) {
	if (size > (u64)(arena->head - arena->base))
		size = (u64)(arena->head - arena->base);
	arena->head -= size;
	return size;
}

bool arenaReserve(arena_t *arena, u64 size, bool mayGrow, bool mayMove) {
	if ((u64)(arena->top - arena->head) >= size)
		return true;
	if (mayGrow)
		return arenaRemap(arena, (u64)(arena->top - arena->base) + size, mayMove);
	return false;
}

bool arenaRemap(arena_t *arena, u64 size, bool mayMove) {
	if (size == 0)
		return arenaUnmap(arena);
	size = ((size + (_arenaPageSizes[0] - 1)) / _arenaPageSizes[0]) * _arenaPageSizes[0];

	void *newBase;
	if (arena->base == NULL)
		newBase = mmap(NULL, size, PROT_RW, MAP_ANONPRIV, 0, 0);
	else
		newBase = mremap(arena->base, arena->top - arena->base, size, mayMove ? MREMAP_MAYMOVE : 0);

	if (newBase == MAP_FAILED)
		return false;
	arena->head = newBase + (arena->head - arena->base);
	arena->base = newBase;
	arena->top = newBase + size;
	return true;
}

void arenaReset(arena_t *arena) {
	arena->head = arena->base;
}

bool arenaUnmap(arena_t *arena) {
	if (arena->base == NULL)
		return true;
	if (munmap(arena->base, arena->top - arena->base))
		return false;
	arena->base = NULL;
	arena->head = NULL;
	arena->top = NULL;
	return true;
}

u64 arenaGetMappedSize(arena_t const *arena) {
	return arena->top - arena->base;
}

u64 arenaGetFreeSize(arena_t const *arena) {
	return arena->top - arena->head;
}

u64 arenaGetUsedSize(arena_t const *arena) {
	return arena->head - arena->base;
}

void arenaUpdatePageSizes() {
	((u64*)_arenaPageSizes)[0] = getpagesize();
}

#ifndef NDEBUG
void arenaDebugPrint(arena_t const *a) {
	printf("%14p - %14p - %14p\n", a->base, a->head, a->top);
	if (a->base == NULL)
		return;
	u32 constexpr width = 14 * 3 + 6 - 12;
	u32 l1 = ((a->head - a->base) * width) / (a->top - a->base);
	u32 l2 = width + 11 + !l1 - printf("\e[1;30;%sm%-*lu", l1 ? "47" : "100", l1, a->head - a->base);
	printf("\e[97;100m%*lu\e[0m /%9lu\n", l2, a->top - a->head, a->top - a->base);
}
#endif // ifndef NDEBUG

#endif // ifdef ARENALLOC_IMPL
#endif // ifndef _ARENALLOC_H_
