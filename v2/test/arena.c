#include <stdio.h>
#include <unistd.h>
#define ARENALLOC_IMPL
#include "arenalloc.h"

int main() {
	arenaUpdatePageSizes();
	printf("\e[33m>> arenaUpdatePageSizes => %lu\e[39m\n", _arenaPageSizes[0]);

	arena_t arena = EMPTY_ARENA;
	puts("\e[33m>> EMPTY_ARENA\e[39m");
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> remap(4096, true)\e[39m");
	arenaRemap(&arena, 4096, true);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(10)\e[39m");
	arenaAlloc(&arena, 10);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(118)\e[39m");
	arenaAlloc(&arena, 118);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(896)\e[39m");
	arenaAlloc(&arena, 896);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(2048)\e[39m");
	arenaAlloc(&arena, 2048);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(1000)\e[39m");
	arenaAlloc(&arena, 1000);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> alloc(1000)\e[39m");
	arenaAlloc(&arena, 1000);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> allocGrow(1000, true)\e[39m");
	arenaAllocGrow(&arena, 1000, true);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> reserve(1000, false, false)\e[39m");
	arenaReserve(&arena, 1000, false, false);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> free(500)\e[39m");
	arenaFree(&arena, 500);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> reset\e[39m");
	arenaReset(&arena);
	arenaDebugPrint(&arena);

	puts("\n\e[33m>> unmap\e[39m");
	arenaUnmap(&arena);
	arenaDebugPrint(&arena);
	return 0;
}
