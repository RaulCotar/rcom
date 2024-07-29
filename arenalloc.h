#ifndef _ARENALLOC_H_
#define _ARENALLOC_H_
#include "types.h"

typedef struct arenalloc {
	void *base; // the start of the arena memory map
	void *head; // the end of the currently used space
	void *end; // the end of the arena memory map
} arenalloc_t; // An arena is considered uninitialized iff its size is 0 (ie. end-base==0).

enum arenalloc_flags {
	ARENALLOC_MAY_MOVE = 1,
	ARENALLOC_MAY_RESIZE_USED = 2,
	ARENALLOC_USE_ALLOCA = 4,
};

/// Allocate (mmap) new arena.
arenalloc_t arena_new(u64 size, enum arenalloc_flags flags);

/// Resize (mremap) an existing arena. Creates a new arena if the old pointer is NULL, frees the old arena if the new size is 0. You may not resize a stack-allocated arena!
int arena_resize(arenalloc_t *arena, u64 size, enum arenalloc_flags flags);

/// Free (munmap) an existing arena NULLifying all ptrs. Releasing an uninitialized arena does nothing. You may not call release a stack-allocated arena!
int arena_release(arenalloc_t *arena);

// In general, the functions above return unitilized arenas (all NULL) to indicate errors. You can then check errno for more info.

/// Allocate memory in the arena. Note that stack-allocated arenas might have their byte order reversed (downwards-growing stack).
void *arena_alloc(arenalloc_t *arena, u64 size, enum arenalloc_flags flags);

/// Free memory, starting from the last allocated byte. The size is capped at the current allocated size. Returns the new allocation head or NULL for uninitilized arenas.
void *arena_free(arenalloc_t *arena, u64 size);

#ifndef NDEBUG
void arena_print(arenalloc_t arena, char const *msg);
#endif

#endif /* _ARENALLOC_H_ */
