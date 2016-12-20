# MyCSS — a pure C CSS parser

MyCSS is a fast CSS Parser implemented as a pure C99 library with the ability to build without dependencies.

* Mailing List: [https://groups.google.com/forum/#!forum/modest-engine](https://groups.google.com/forum/#!forum/modest-engine)
* IRC: [#modest-engine on `irc.freenode.net <http://freenode.net>`](http://webchat.freenode.net?channels=%23modest-engine)
* Or you can ask questions on [stackoverflow.com](https://stackoverflow.com/questions/ask?tags=myhtml) by myhtml tag

By default MyCSS depends of [MyHTML] library. This is one of module of the [Modest] project.

## Features

- Modules: [CSS Syntax], [Selectors Level 4], [Namespace], [Values], [Box] and other
- Two API - [high] and [low]-level
- Support [parsing by chunks]
- Support 39 character encoding by specification [encoding.spec.whatwg.org]
- Support detect encodings
- C99 support 
- See [Modest] for full capacity

## Support encodings for InputStream

See MyHTML [support encodings]

## Support encodings for output

**Program working in UTF-8 and returns all in UTF-8**

## Dependencies

[MyHTML]

## Installation

See [INSTALL.md](https://github.com/lexborisov/mycss/blob/master/INSTALL.md)

## External Bindings and Wrappers

All in our hands!

## Examples

See [examples] directory

**Simple example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mycss/api.h>

int main(int argc, const char * argv[])
{
    char *css = "#ident [name=\"best-name\"] {rgba(0, 0, 0, 0.1);}";
    
    // base init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // current entry, work init
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


[CSS Syntax]: https://www.w3.org/TR/css-syntax-3/
[high]: https://github.com/lexborisov/mycss/blob/master/include/mycss/api.h
[low]: https://github.com/lexborisov/mycss/tree/master/include/mycss
[MyHTML]: https://github.com/lexborisov/myhtml
[parsing by chunks]: https://github.com/lexborisov/mycss/blob/master/examples/tokenizer_chunk_high_level.c
[encoding.spec.whatwg.org]: https://encoding.spec.whatwg.org/
[Modest]: https://github.com/lexborisov/Modest
[support encodings]: https://github.com/lexborisov/myhtml#support-encodings-for-inputstream
[examples]: https://github.com/lexborisov/mycss/blob/master/examples/
[Selectors Level 4]: https://drafts.csswg.org/selectors/
[Namespace]: https://drafts.csswg.org/css-namespaces/
[Values]: https://drafts.csswg.org/css-values/
[Box]: https://drafts.csswg.org/css-box/
[LICENSE]: https://github.com/lexborisov/mycss/blob/master/LICENSE
