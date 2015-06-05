#
# INIPARSER
#



###
### PROGRAM CHECK Macro
###
#
# Args:
#   1: program name to check, exact case
#   2: variable to define, usually upper case (is prefixed with SNOOPY_PROG_ and suffixed with _FOUND)
#   3: additional info if program is not found
#
AU_DEFUN([INIPARSER_PROG_CHECK],
[
    AC_CHECK_PROG(INIPARSER_PROG_$2_FOUND, $1, yes)
    if test x"$INIPARSER_PROG_$2_FOUND" != x"yes" ; then
        AC_MSG_ERROR([Please install "$1" program. Reason: $3])
    fi
])
