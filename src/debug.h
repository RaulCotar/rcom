#ifndef _RCOM_DEBUG_H_
#define _RCOM_DEBUG_H_

#ifndef NDEBUG

#include <assert.h> // IWYU pragma: export

void stacktrace(void);

#ifndef RCOM_DEBUG_IMPL

#include <execinfo.h>
#include <unistd.h>

void stacktrace(void) {
	void *tracebuf[512];
	int n = backtrace(tracebuf, 512);
	backtrace_symbols_fd(tracebuf, n, 2);
}

#endif /* RCOM_DEBUG_IMPL */
#endif /* NDEBUG */
#endif /* _RCOM_DEBUG_H_ */
