# Raul's Common header-only Library

## [arenalloc.h](src/arenalloc.h)

A simple but versatile arena/stack allocator.

## [mman.h](src/mman.h)

Defines some quality-of-life macros for `sys/mman.h`.

## [types.h](src/types.h)

The typedefs I usually use in my code.

## [debug.h](src/debug.h)

Stacktrace printing function.

# Changelog

### v2.1.0
- rename include guard macros
- mman: `#undef __USE_GNU` after `<sys/mman.h>` import

### v2.0.0
Complete rewrite of the library, mainly for arenalloc.
