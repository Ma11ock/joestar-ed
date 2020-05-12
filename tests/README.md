## Testing

The tests use CMake instead of the autoconf build system that the rest
of `joestar` uses.

### Dependencies (make sure they are on your PATH)
* CMake
* Make
* Google Test (make sure you have its dev package on Linux distros that have those)
* C compiler (gcc/clang)

### How to

From the project root, cd into the tests directory (where this README lives)

#### Linux (Probably also MacOS)
```shell
cmake ..
make check
```

#### Windows (with MinGW)
```shell
cmake .. "-GMinGW Makefiles"
mingw32-make check
```