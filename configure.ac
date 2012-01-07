AC_PREREQ([2.65])
AC_INIT([iniparser], [3.0], [ndevilla@free.fr])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)

AM_INIT_AUTOMAKE([1.8 foreign])

SHARED_VERSION_INFO="3:0:0"
AC_SUBST(SHARED_VERSION_INFO)

AC_ARG_ENABLE(la-files,
	[AS_HELP_STRING([--disable-la-files],[Do NOT install GNU libtool's la files])],,
	[enable_la_files=yes])

AM_CONDITIONAL(NO_LA_FILES,[test "$enable_la_files" = no])

AC_PATH_PROG(DOXYGEN,doxygen,no)
AM_CONDITIONAL(HAVE_DOXYGEN,[test .$DOXYGEN != .no])

# Checks for programs.
AC_PROG_CC

LT_INIT
# Checks for header files.
AC_CHECK_HEADERS([ ctype.h stdio.h stdlib.h string.h ])

# Checks for library functions.
AC_CHECK_FUNCS([strcmp])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/libiniparser.pc])
AC_CONFIG_FILES([test/Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([doc/iniparser.dox])
AC_OUTPUT
