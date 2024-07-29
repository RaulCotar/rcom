/// Common compiler builtins. (mostly gcc and clang)
#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#undef  NULL
#define NULL		nullptr
#ifndef alloca
#define alloca		__builtin_alloca
#endif
#define calloc		__builtin_calloc
#define malloc		__builtin_malloc
#define realloc		__builtin_realloc
#define free		__builtin_free
#define memcpy		__builtin_memcpy
#define memcmp		__builtin_memcmp
#define strcpy		__builtin_strcpy
#define strncpy		__builtin_strncpy
#define strdup		__builtin_strdup
#define strndup		__builtin_strndup
#define strlen		__builtin_strlen
#define strcmp 		__builtin_strcmp
#define strncmp		__builtin_strncmp
#define abs			__builtin_abs
#define likely(X)	__builtin_expect(!!(X), 1)
#define unlikely(X)	__builtin_expect(!!(X), 0)
#define MIN(A,B) \
   ({ __typeof__ (A) _A = (A); \
       __typeof__ (B) _B = (B); \
     _A <= _B ? _A : _B; })
#define MAX(A,B) \
   ({ __typeof__ (A) _A = (A); \
       __typeof__ (B) _B = (B); \
     _A > _B ? _A : _B; })

// #ifndef unreachable
#define unreachable __builtin_unreachable
// #endif

#define sizeof_member(_T, _M) sizeof( ((_T *)0)->_M )
#define typeof_member(_T, _M) typeof( ((_T *)0)->_M )

#endif /* _BUILTINS_H_ */
