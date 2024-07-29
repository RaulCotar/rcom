// Stuff useful when debugging - assertions, stack staces, etc.
#ifndef _DEBUG_H_
#define _DEBUG_H_
#include <assert.h> // IWYU pragma: export
#ifndef NDEBUG
#include <stdio.h> // IWYU pragma: export

void stacktrace(void);

#endif /* NDEBUG */
#endif /* _DEBUG_H_ */
