# Fast C/C++ CSS Parser (Cascading Style Sheets)

MyCSS is a fast CSS Parser implemented as a pure C99 library with the ability to build without dependencies.

By default MyCSS depends of one [MyHTML] library.

## Now

```text
The current version is 0.0.2 - this is a alpha version
Release will have major version number 1
```

## Features

- By [CSS3 specification]
- Two API - [high] and [low]-level
- Support [parsing by chunks]
- Support 39 character encoding by specification [encoding.spec.whatwg.org]
- Support detect encodings
- C99 support

## Support encodings for InputStream

See MyHTML [support encodings]

## Support encodings for output

**Program working in UTF-8 and returns all in UTF-8**

## Build and Installation

**Make**

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

```bash
cp lib/* /usr/local/lib
cp -r include/* /usr/local/include
```

**CMake**

In mycss/project directory:

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

## Dependencies

By default only on [MyHTML]

## In other languages, external bindings

All in our hands!

## Examples

See [examples] directory

## Future

- [Selectors Level 4]
- [Media Queries Level 4]
- [CSS Backgrounds and Borders Module Level 3]
- [CSS Color Module Level 3]
- [CSS Fonts Module Level 3]
- and many other modules for Cascading Style Sheets

**Simple example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mycss/api.h>

int main(int argc, const char * argv[])
{
    // work data
    char *css = "#ident [name=\"best-name\"] {rgba(0, 0, 0, 0.1);}";
    
    // base init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // currenе entry work init
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    mycss_parse(entry, MyHTML_ENCODING_UTF_8, css, strlen(css));

    // release resurces
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    return 0;
}
```

## AUTHOR

Alexander Borisov <lex.borisov@gmail.com>

## COPYRIGHT AND LICENSE

Copyright (C) 2016 Alexander Borisov

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA

See the [LICENSE] file.


[CSS3 specification]: https://www.w3.org/TR/css-syntax-3/
[MyHTML]: https://github.com/lexborisov/myhtml
[parsing by chunks]: https://github.com/lexborisov/mycss/blob/master/examples/tokenizer_chunk_high_level.c
[encoding.spec.whatwg.org]: https://encoding.spec.whatwg.org/
[support encodings]: https://github.com/lexborisov/myhtml#support-encodings-for-inputstream
[examples]: https://github.com/lexborisov/mycss/blob/master/examples/
[Selectors Level 4]: https://www.w3.org/TR/selectors4/
[Media Queries Level 4]: https://www.w3.org/TR/mediaqueries-4/
[CSS Backgrounds and Borders Module Level 3]: https://www.w3.org/TR/css3-background/
[CSS Color Module Level 3]: https://www.w3.org/TR/css3-color/
[CSS Fonts Module Level 3]: https://www.w3.org/TR/css-fonts-3/
[LICENSE]: https://github.com/lexborisov/mycss/blob/master/LICENSE
