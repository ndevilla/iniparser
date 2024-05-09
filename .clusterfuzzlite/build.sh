#!/bin/bash
find . -name "*.c" -exec $CC $CFLAGS -I./src -c {} \;
find . -name "*.o" -exec cp {} . \;

llvm-ar rcs libfuzz.a *.o


$CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/fuzzer.c \
  -Wl,--whole-archive $SRC/iniparser/libfuzz.a -Wl,--allow-multiple-definition \
  -I$SRC/iniparser/src  -o $OUT/fuzzer
