[![Build Status](https://travis-ci.org/touilleMan/iniparser4.svg?branch=master)](https://travis-ci.org/touilleMan/iniparser4)

# Iniparser 4 #

## 0 - Foreword

[Iniparser4](https://github.com/touilleMan/iniparser4) is a cordial
fork of [Iniparser](https://github.com/ndevilla/iniparser).
Iniparser4 is aimed at beeing a new cuting edge version of this
project while the original Iniparser will be kept as legacy for
compatibility reasons.

## I - Overview

This modules offers parsing of ini files from the C level.
See a complete documentation in HTML format, from this directory
open the file html/index.html with any HTML-capable browser.

Key features :

 - Small : around 1500 sloc inside 4 files (2 .c and 2 .h)
 - Portable : no dependancies, written in `-ansi -pedantic` C89
 - Fully reintrant : easy to make it thread-safe (just surround
   library calls by mutex)

## II - Building project

A simple `make` at the root of the project should be enough to get the static
(i.e. `libiniparser.a`) and shared (i.e. `libiniparser.so.0`) libraries compiled.

You should consider trying the following rules too :

 - `make check` : run the unitary tests
 - `make example` : compile the example, run it with `./example/iniexample`

## III - License

This software is released under MIT License.
See LICENSE for full informations
