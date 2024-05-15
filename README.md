[![Build Status](https://travis-ci.org/ndevilla/iniparser.svg?branch=master)](https://travis-ci.org/ndevilla/iniparser)

# Iniparser 4 #

Changes in May 2024:
We moved to [iniparser/iniparser](https://gitlab.com/iniparser/iniparser)!
Go there for:

- [New Issues](https://gitlab.com/iniparser/iniparser/-/issues/new)
- [Merge Requests](https://gitlab.com/iniparser/iniparser/-/merge_requests/new)

Changes in March 2024:
@lmoellendorf is taking over maintenance for this project. The objective is to get all known issues fixed and released as v4.2.
After that, this project will be frozen here as v4.2 and development should continue as v5, probably on gitlab. Pointers will be updated then.

Thanks a million times to Lars for his tremendous work and help in keeping this library alive!

Many thanks to @touilleman for his exceptional contributions and efforts for maintaining this project for a decade. Manu, you rock!

## I - Overview

This modules offers parsing of ini files from C.
See the [complete documentation](https://iniparser.gitlab.io/iniparser-meta/) in HTML format.


Key features:

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependancies, written in `-ansi -pedantic` C89
 - Fully re-entrant : easy to make it thread-safe (just surround
   library calls by mutex)

## II - Building project

A simple `make` at the root of the project should be enough to get the static
(i.e. `libiniparser.a`) and shared (i.e. `libiniparser.so.0`) libraries compiled.

You should consider trying the following rules too :


 - `make check` : run unit tests
 - `make example` : compile the example, run it with `./example/iniexample`
 - `make docs` : generate the documentation, open the file `html/index.html` with any HTML-capable browser.

For installation and packaging see [iniparser-meta](https://gitlab.com/iniparser/iniparser-meta).

## III - License

This software is released under MIT License.
See LICENSE for more details

## IV - Versions

Current version is 4.2. Version 4.0 introduces breaking changes in the api.
Older versions 3.1 and 3.2 with the legacy api are available as tags.

## V - FAQ

See [FAQ-en.md](FAQ-en.md) in this directory for answers to Frequently Asked Questions.

还有简化中国翻译在[FAQ-zhcn.md](FAQ-zhcn.md).
