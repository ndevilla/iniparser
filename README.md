[![pipeline status](https://gitlab.com/iniparser/iniparser/badges/main/pipeline.svg)](https://gitlab.com/iniparser/iniparser/-/commits/main)
[![latest release](https://gitlab.com/iniparser/iniparser/-/badges/release.svg)](https://gitlab.com/iniparser/iniparser/-/releases)

# Iniparser 4 #

Changes in May 2024:
We moved to [iniparser/iniparser](https://gitlab.com/iniparser/iniparser)!
Go there for:

- [New Issues](https://gitlab.com/iniparser/iniparser/-/issues/new)
- [Merge Requests](https://gitlab.com/iniparser/iniparser/-/merge_requests/new)

Changes in March 2024:
@lmoellendorf took over maintenance for this project.

- [X] The objective was to get all known issues fixed and released as v4.2.
- [X] After that, this project has been frozen on Github as v4.2.1 and
- [ ] development should continue as v5, ~probably~ on gitlab.
- [X] Pointers have been updated.

Thanks a million times to Lars for his tremendous work and help in keeping this
library alive!

Many thanks to @touilleman for his exceptional contributions and efforts for
maintaining this project for a decade. Manu, you rock!

## I - Overview

This modules offers parsing of ini files from C.

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

Or see the [complete documentation](https://iniparser.gitlab.io/iniparser/)
in online.

## VI - License

This software is released under MIT License.
See [LICENSE](LICENSE) for more details

## VII - Versions

[![latest release](https://gitlab.com/iniparser/iniparser/-/badges/release.svg)](https://gitlab.com/iniparser/iniparser/-/releases)

- Since v4.2.x [Semantic Versioning](https://semver.org/) is applied.
- Version 4.0 introduced breaking changes in the api.
- Older versions 3.1 and 3.2 with the legacy api are available as tags.

## VIII - FAQ

See [FAQ-en.md](FAQ-en.md) in this directory for answers to Frequently Asked
Questions.

还有简化中国翻译在[FAQ-zhcn.md](FAQ-zhcn.md).
