# Rcom - Raul's Common Library

## Building and installing
This project uses a simple hand-written [Makefile](Makefile). Some recipes use `sudo` to copy or remove files from the install diectories. Nothing tricky is going on, so feel free to change the makefile as you wish.
- `make` - build, install and run everything
- `make install` - build and install the libs and headers
- `make test` - compile and run the tests
- `make clean` - remove all installed and generated files
- `make help` - more detailed help message

## Using the library

```c
// example.c
#include <rcom/foo.h> // fictitious header

int main() {
	foo();
	return 0;
}
```

You should make sure the values of the `$LIB_DIR` and `$H_DIR` make vars appear in your `$LD_LIBRARY_PATH` and `$CPATH` env vars or in you compiler's default search paths, otherwise you might have to use the `-I`, `-L` and/or `-Wl,-rpath` flags to manually specify the correct paths. Below are some examples of static and dynamic linking:
```bash
cc -lrcom -Wl,-rpath=/usr/local/lib example.c # explicit dynamic linking path
cc -lrcom -static exmaple.c # static linking for everything
cc example.c /usr/local/lib/librcom.a # static linking only for Rcom
cc -Wl,-Bstatic -lrcom -Wl,-Bdynamic example.c # static linking only for Rcom
```
