#ifndef NDEBUG
#include <execinfo.h>
#include "debug.h"

// TODO: remove malloc dependency
void stacktrace(void) {
	void *tracebuf = __builtin_malloc(sizeof(void*) * 512);
	int n = backtrace(tracebuf, 512);
	backtrace_symbols_fd(tracebuf, n, 2);
	__builtin_free(tracebuf);
}

#endif // ifndef NDEBUG