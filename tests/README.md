## Testing

The tests use CMake instead of the autoconf build system that the rest
of `joestar` uses.

### Dependencies (make sure they are on your PATH)
* CMake
* Make
* Google Test (make sure you have its dev package on Linux distros that have those)
* C compiler (gcc/clang)

You will also have to build joestar the normal way (see the README in the
project root) in order to generate some header files that it needs.

### How to

From the project root, cd into the tests directory (where this README lives)

#### Unix
```shell
cmake ..
make check
```

#### Windows (with MinGW)
```shell
cmake .. "-GMinGW Makefiles"
mingw32-make check
```