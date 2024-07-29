// Thin wrapper for <sys/mman.h>
#ifndef _MMAN_H_
#define _MMAN_H_
#define __USE_GNU // for mremap
#include <sys/mman.h> // IWYU pragma: export

#define PROT_RW (PROT_READ|PROT_WRITE)
#define MAP_ANONPRIV (MAP_ANONYMOUS|MAP_PRIVATE)

#endif /* _MMAN_H_ */
