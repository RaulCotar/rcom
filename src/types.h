#ifndef _RCOM_TYPES_H_
#define _RCOM_TYPES_H_

#include <stdint.h> // IWYU pragma: export

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t	 u8;
typedef int64_t	 i64;
typedef int32_t	 i32;
typedef int16_t	 i16;
typedef int8_t	 i8;
typedef double	 f64;
typedef float	 f32;

#ifdef RCOM_EXTRA_TYPES
typedef __int128 i128;
typedef unsigned __int128 u128;
typedef __float128 f128;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* _RCOM_TYPES_H_ */
