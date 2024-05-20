[![Build Status](https://travis-ci.org/ndevilla/iniparser.svg?branch=master)](https://travis-ci.org/ndevilla/iniparser)

# Iniparser 4 #

Changes in May 2024:
We moved to [iniparser/iniparser](https://gitlab.com/iniparser/iniparser)!
Go there for:

- [New Issues](https://gitlab.com/iniparser/iniparser/-/issues/new)
- [Merge Requests](https://gitlab.com/iniparser/iniparser/-/merge_requests/new)

Changes in March 2024:
@lmoellendorf is taking over maintenance for this project. The objective is to
get all known issues fixed and released as v4.2.  After that, this project will
be frozen here as v4.2 and development should continue as v5, probably on
gitlab. Pointers will be updated then.

Thanks a million times to Lars for his tremendous work and help in keeping this
library alive!

Many thanks to @touilleman for his exceptional contributions and efforts for
maintaining this project for a decade. Manu, you rock!

## I - Overview

This modules offers parsing of ini files from C.
See the [complete documentation](https://iniparser.gitlab.io/iniparser-meta/)
in HTML format.


Key features:

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependancies, written in `-pedantic` C
 - Fully re-entrant : easy to make it thread-safe (just surround
   library calls by mutex)

## II - Building project

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

- `BUILD_TESTS`
- `BUILD_EXAMPLES`

From within build directory execute `ccmake ..` to see all.

## III - Tests

```
mkdir build
cd build
cmake -DBUILD_TESTS ..
make all
```

While still in the build project you can run the tests by calling `ctest`.
Test output can be found in build directory under
`Testing/Temporary/LastTest.log`.

## IV - Examples

To build the examples:

```
mkdir build
cd build
cmake -DBUILD_EXAMPLES ..
make all
```

From the build directory run the examples with:

 - `./iniexample`
 - `./iniwrite`
 - `./parse ../example/twisted.ini`

## V - Documentation

Documentation is build by default and can be found in build directory under
`html`.

Open the file `html/index.html` with any HTML-capable browser.

## VI - License

This software is released under MIT License.
See LICENSE for more details

## VII - Versions

Current version is 4.2.x. Version 4.0 introduces breaking changes in the api.
Older versions 3.1 and 3.2 with the legacy api are available as tags.

## VIII - FAQ

See [FAQ-en.md](FAQ-en.md) in this directory for answers to Frequently Asked
Questions.

还有简化中国翻译在[FAQ-zhcn.md](FAQ-zhcn.md).
