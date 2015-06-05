###
### iniparser's configure.ac
###



### Initialize autotools
#
AC_PREREQ([2.63])
AC_INIT(
    [iniparser],
    [4.0.0],
    [https://github.com/ndevilla/iniparser/issues/],
    [iniparser],
    [https://github.com/ndevilla/iniparser/])


### Configure autotools
#
AC_CONFIG_SRCDIR([src/iniparser.c])
AC_CONFIG_AUX_DIR([build/aux])
AC_CONFIG_MACRO_DIR([build/m4])
m4_include([build/iniparser.m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Werror])


### Silent make output
#
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


### Enable libtool
#
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT


### Generate this header file
#
AC_CONFIG_HEADERS([config.h])


### Checks for programs
#
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB


### Checks for header files.
#
AC_CHECK_HEADERS([ \
    ctype.h \
    stdio.h \
    stdlib.h \
    string.h \
    unistd.h \
], [], [
    AC_MSG_ERROR([A header file is missing. Unable to continue.])
])


### Checks for typedefs, structures, and compiler characteristics.
#
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T


### Checks for library functions.
#
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ \
    atof \
    calloc \
    fclose \
    feof \
    fgets \
    fopen \
    fprintf \
    free \
    isspace \
    malloc \
    memcpy \
    memmove \
    memset \
    snprintf \
    sprintf \
    sscanf \
    strchr \
    strcpy \
    strcmp \
    strlen \
    strncmp \
    strtol \
], [], [
    AC_MSG_ERROR([A function is missing. Unable to continue.])
])


### Generate these (Make)files
#
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 example/Makefile
                 src/Makefile
                 test/Makefile
])



# ==============================================================================
# ======= START: Options =======================================================
# ==============================================================================

### Option: --disable-documentation
#
AC_ARG_ENABLE(documentation,
    [AC_HELP_STRING(
        [--disable-documentation],
        [disables building documentation. If enabled, 'doxygen' program is required. [default=enabled]]
    )],
    [
        if test "x$enableval" == "xyes"; then
            documentation_enabled="yes"
        elif test "x$enableval" == "xno"; then
            documentation_enabled="no"
        else
            AC_MSG_ERROR([--enable/--disable-documentation does not take an argument. Got: $enableval])
        fi
    ],
    [documentation_enabled="yes"]
)

AM_CONDITIONAL([DOCUMENTATION_ENABLED], [test "x$documentation_enabled" == "xyes"])
AS_IF([test "x$documentation_enabled" = "xyes"],
    INIPARSER_PROG_CHECK([doxygen],  [DOXYGEN],  [Needed for creating documentation.])
)




### Option: --enable-subpackage
#
AC_ARG_ENABLE(subpackage,
    [AC_HELP_STRING(
        [--enable-subpackage],
        [enables building iniparser as subpackage of another project. Does not build nor install shared library, only static which is then linked into main project. [default=disabled]]
    )],
    [
        if test "x$enableval" == "xyes"; then
            subpackage_enabled="yes"
        elif test "x$enableval" == "xno"; then
            subpackage_enabled="no"
        else
            AC_MSG_ERROR([--enable/--disable-subpackage does not take an argument. Got: $enableval])
        fi
    ],
    [subpackage_enabled="no"]
)

AM_CONDITIONAL([SUBPACKAGE_ENABLED], [test "x$subpackage_enabled" == "xyes"])

# ==============================================================================
# ======= END: Options =========================================================
# ==============================================================================



### Produce output
#
AC_OUTPUT
