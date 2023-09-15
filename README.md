[![Build Status](https://travis-ci.org/ndevilla/iniparser.svg?branch=master)](https://travis-ci.org/ndevilla/iniparser)

# Iniparser 4 #

*Sept 2023: We are looking for volunteers to help us maintain this library!*

Maintaining open-source projects takes time we cannot always easily find. If you feel up to the task and already know this library well, please get in touch by email (ndevilla AT gmail) and let's figure it out!


## I - Overview

This modules offers parsing of ini files from the C level.
See a complete documentation in HTML format, from this directory
open the file html/index.html with any browser.

Key features :

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependencies, written in `-ansi -pedantic` C89
 - Easy to make it thread-safe, just surround library calls by a mutex.

## II - Building project

A simple `make` at the root of the project should be enough to get the static
(i.e. `libiniparser.a`) and shared (i.e. `libiniparser.so.0`) libraries compiled.

You should consider trying the following rules too :

 - `make check` : run unit tests
 - `make example` : compile the example, run it with `./example/iniexample`

## III - License

This software is released under MIT License.
See LICENSE for full informations

## IV - Versions

Current version is 4.1. Version 4.0 introduces breaking changes in the api.
Older versions 3.1 and 3.2 with the legacy api are available as tags.

## V - FAQ

See [FAQ-en.md](FAQ-en.md) in this directory for answers to Frequently Asked Questions.

还有简化中国翻译在[FAQ-zhcn.md](FAQ-zhcn.md).
