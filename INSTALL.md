# MyCSS: Build and Installation

## make

In root directory:
```bash
make
```

If successful copy lib/* and include/* at the right place for you

Flags that can be passed to make:
- `MyCSS_OPTIMIZATION_LEVEL=-O2` set compiler optimization level. Default: -O2
- `MyCSS_BUILD_WITHOUT_THREADS=YES` build without POSIX Threads. Default: NO

*for example*
```bash
make MyCSS_BUILD_WITHOUT_THREADS=NO
```

and copy to the right place for you
```bash
cp lib/* /usr/local/lib
cp -r include/* /usr/local/include
```

## cmake

In `project` directory:
```bash
cmake .
make
sudo make install
```

Flags that can be passed to CMake:
- `MyCSS_OPTIMIZATION_LEVEL=-O2` set compiler optimization level. Default: -O2
- `CMAKE_INSTALL_LIBDIR=lib` set path to install created library. Default: lib
- `MyCSS_BUILD_SHARED=ON` build shared library. Default: ON
- `MyCSS_BUILD_STATIC=ON` build static library. Default: ON
- `MyCSS_INSTALL_HEADER=OFF` install header files. Default OFF
- `MyCSS_BUILD_WITHOUT_THREADS=YES` build without POSIX Threads. Default: NO

*for example*
```bash
cmake . -DCMAKE_INSTALL_LIBDIR=lib64 -DMyCSS_INSTALL_HEADER=ON
```
