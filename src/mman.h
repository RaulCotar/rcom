// Thin wrapper for <sys/mman.h>
#ifndef _RCOM_MMAN_H_
#define _RCOM_MMAN_H_

#define __USE_GNU // for mremap
#include <sys/mman.h> // IWYU pragma: export
#undef __USE_GNU

#define PROT_RW (PROT_READ|PROT_WRITE)
#define MAP_ANONPRIV (MAP_ANONYMOUS|MAP_PRIVATE)

#endif /* _RCOM_MMAN_H_ */
