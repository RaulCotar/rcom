/// Common typedefs.
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h> // IWYU pragma: export

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t	 u8;
typedef int64_t	 i64;
typedef int32_t	 i32;
typedef int16_t	 i16;
typedef int8_t	 i8;
typedef u32		 uint;
typedef u64		 ulong;
typedef float	 f32;
typedef double	 f64;

#undef NULL
#define NULL nullptr

#endif /* _TYPES_H_ */
