/*
  ssize_t is defined in POSIX and is absent in ANSI C89. To keep the public
  interface intact, we have to define our own ssize_t on non-POSIX platforms.
*/
typedef int ssize_t;
