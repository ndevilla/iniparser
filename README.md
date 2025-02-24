[![pipeline status](https://gitlab.com/iniparser/iniparser/badges/main/pipeline.svg)](https://gitlab.com/iniparser/iniparser/-/commits/main)
[![latest release](https://gitlab.com/iniparser/iniparser/-/badges/release.svg)](https://gitlab.com/iniparser/iniparser/-/releases)

# iniParser 4 #

Changes in May 2024:
We moved to [iniparser/iniparser](https://gitlab.com/iniparser/iniparser)!
Go there for:

- [New Issues](https://gitlab.com/iniparser/iniparser/-/issues/new)
- [Merge Requests](https://gitlab.com/iniparser/iniparser/-/merge_requests/new)

Changes in March 2024:
\@lmoellendorf took over maintenance for this project.

- [X] The objective was to get all known issues fixed and released as v4.2.
- [X] After that, this project has been frozen on Github as v4.2.1 and
- [ ] development should continue as v5, ~probably~ on gitlab.
- [X] Pointers have been updated.

Thanks a million times to Lars for his tremendous work and help in keeping this
library alive!

Many thanks to \@touilleman for his exceptional contributions and efforts for
maintaining this project for a decade. Manu, you rock!

## Overview

This modules offers parsing of ini files from C.

Key features:

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependancies, written in `-pedantic` C
 - Fully re-entrant : easy to make it thread-safe (just surround
   library calls by mutex)

## Installation

iniParser is available in a number of package repositories:

[![Packaging status](https://repology.org/badge/vertical-allrepos/iniparser.svg)](https://repology.org/project/iniparser/versions)

## MinGW

In the instructions below, replace `make all` by `ninja` if you are using
MinGW as build system. Of course you will need to install
[ninja](https://ninja-build.org/) to use it.

## Building project

This project uses CMake as build system. Use these command at the root or the
project to get the static (i.e. `libiniparser.a`) and shared (i.e.
`libiniparser.so`) libraries compiled:

```
mkdir build
cd build
cmake ..
make all
```

There are some CMake option which are `OFF` by default:

- `BUILD_TESTING`
- `BUILD_EXAMPLES`
- `BUILD_DOCS`

These CMake options are `ON` by default:

- `BUILD_SHARED_LIBS`
- `BUILD_STATIC_LIBS`

From within build directory execute `ccmake ..` to see all.


## Tests

```
mkdir build
cd build
cmake -DBUILD_TESTING=ON ..
make all
```

While still in the build project you can run the tests by calling `ctest`.
Test output can be found in build directory under
`Testing/Temporary/LastTest.log`.

Each time `cmake` is executed in an empty build directory it will try to clone
the unit test framework [Unity](https://www.throwtheswitch.org/unity).
To avoid unnecessary downloads you may place a copy outside your build
directory and point cmake to it.

To do so first clone Unity in a project of your choice:

```
git clone https://github.com/throwtheswitch/unity.git
```
Now change into your build directory and pass `FETCHCONTENT_SOURCE_DIR_UNITY`
to `cmake` (adjust the path to unity to your local setup):
```
mkdir build
cd build
cmake -DBUILD_TESTING=ON -DFETCHCONTENT_SOURCE_DIR_UNITY=../../3rparty/unity ..
make all
```
Now CMake will try to use the sources in this directory and fall back to
cloning if it cannot find them there.


## Examples

To build the examples:

```
mkdir build
cd build
cmake -DBUILD_EXAMPLES=ON ..
make all
```

From the build directory run the examples with:

 - `./iniexample`
 - `./iniwrite`
 - `./parse ../example/twisted.ini`


## Documentation

The library is completely documented in its header file.

To build the documentation [doxygen](https://www.doxygen.org/index.html) has be
installed. Documentation can build and be found in build directory under
`html`:

```
mkdir build
cd build
cmake -DBUILD_DOCS=ON ..
make all
```

Open the file `html/index.html` with any HTML-capable browser.

Or see the [complete documentation](https://iniparser.gitlab.io/iniparser/)
online.


## License

This software is released under MIT License.
See [LICENSE](LICENSE) for more details


## Versions

[![latest release](https://gitlab.com/iniparser/iniparser/-/badges/release.svg)](https://gitlab.com/iniparser/iniparser/-/releases)

- Since v4.2.x [Semantic Versioning](https://semver.org/) is applied.
- Version 4.0 introduced breaking changes in the api.
- Older versions 3.1 and 3.2 with the legacy api are available as tags.


## FAQ

See [FAQ-en.md](FAQ-en.md) in this directory for answers to Frequently Asked
Questions.

还有简化中国翻译在[FAQ-zhcn.md](FAQ-zhcn.md).


## Details

### Introduction

iniParser is a simple C library offering ini file parsing services.
The library is pretty small (less than 1500 lines of C) and robust, and does
not depend on any other external library to compile. It is written in C and
should compile on most platforms without difficulty.


### What is an ini file?

An ini file is an ASCII file describing simple parameters (character strings,
integers, floating-point values or booleans) in an explicit format, easy to use
and modify for users.

An ini file is segmented into Sections, declared by the following syntax:

```ini
[Section Name]
```

i.e. the section name enclosed in square brackets, alone on a line. Sections
names are allowed to contain any character but square brackets or linefeeds.

In any section are zero or more variables, declared with the following syntax:

```ini
Key = value ; comment
```

The key is any string (possibly containing blanks). The value is any character
on the right side of the equal sign. Values can be given enclosed with quotes.
If no quotes are present, the value is understood as containing all characters
between the first and the last non-blank characters before the comment. The
following declarations are identical:

```ini
Hello = "this is a long string value" ; comment
Hello = this is a long string value ; comment
```

The semicolon and comment at the end of the line are optional. If there is a
comment, it starts from the first character after the semicolon up to the end
of the line.

Multi-line values can be provided by ending the line with a backslash (`\`).

```ini
Multiple = Line 1 \
Line 2 \
Line 3 \
Line 4 ; comment
```

This would yield: "multiple" <- "Line1 Line2 Line3 Line4"

Comments in an ini file are:

- Lines starting with a hash sign
- Blank lines (only blanks or tabs)
- Comments given on value lines after the semicolon (if present)


### Using the library

To use the library in your programs, add the following line on top of your
module:

```c
#include "iniparser.h"
```

And link your program with the iniParser library by adding `-liniparser` to the
compile line.

See the file example/example.c for an example.

iniParser is an C library. If you want to compile it with a C++ compiler have
to include the extern "C" hack to include the header:

```c
#ifdef __cplusplus
extern "C"
{
#endif
#include "iniparser.h"
#ifdef __cplusplus
}
#endif
```
Comments are discarded by the parser. Then sections are identified, and in each
section a new entry is created for every keyword found. The keywords are stored
with the following syntax:

```c
[Section]
Keyword = value ; comment
```

is converted to the following key pair:

```c
("section:keyword", "value")
```

This means that if you want to retrieve the value that was stored in the
section called `Pizza`, in the keyword `Cheese`, you would make a request to
the dictionary for `"pizza:cheese"`. All section and keyword names are
converted to lowercase before storage in the structure. The value side is
conserved as it has been parsed, though.

Section names are also stored in the structure. They are stored using as key
the section name, and a NULL associated value. They can be queried through
`iniparser_find_entry()`.

To launch the parser, use the function called `iniparser_load()`, which takes
an input file name and returns a newly allocated `dictionary` structure. This
latter object should remain opaque to the user and only accessed through the
following accessor functions:

- `iniparser_getstring()`
- `iniparser_getint()`
- `iniparser_getdouble()`
- `iniparser_getboolean()`

Finally, discard this structure using `iniparser_freedict()`.

All values parsed from the ini file are stored as strings. The accessors are
just converting these strings to the requested type on the fly, but you could
basically perform this conversion by yourself after having called the string
accessor.

Notice that `iniparser_getboolean()` will return an integer (0 or 1), trying to
make sense of what was found in the file. Strings starting with "y", "Y", "t",
"T" or "1" are considered true values (return 1), strings starting with "n",
"N", "f", "F", "0" are considered false (return 0). This allows some
flexibility in handling of boolean answers.

If you want to add extra information into the structure that was not present in
the ini file, you can use `iniparser_set()` to insert a string.

If you want to add a section to the structure, add a key with a NULL value.
Example:

```c
iniparser_set(ini, "section", NULL);
iniparser_set(ini, "section:key1", NULL);
iniparser_set(ini, "section:key2", NULL);
```


### A word about the implementation

The dictionary structure is a pretty simple dictionary implementation which
might find some uses in other applications. If you are curious, look into the
source.


### Known defects

The dictionary structure is extremely unefficient for searching as keys are
sorted in the same order as they are read from the ini file, which is
convenient when dumping back to a file. The simplistic first-approach linear
search implemented there can become a bottleneck if you have a very large
number of keys.

People who need to load large amounts of data from an ini file should
definitely turn to more appropriate solutions: sqlite3 or similar. There are
otherwise many other dictionary implementations available on the net to replace
this one.
