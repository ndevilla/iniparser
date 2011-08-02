# Copyright 2011 by Alexander Dahl <post@lespocky.de>

include(CheckIncludeFile)
check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("ctype.h" HAVE_CTYPE_H)
check_include_file("unistd.h" HAVE_UNISTD_H)

set(INIPARSER_SRC
    dictionary.c
    iniparser.c
)

add_library(${PROJECT_NAME} SHARED ${INIPARSER_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PACKAGE_MAJOR_VERSION}.${PACKAGE_MINOR_VERSION}
    SOVERSION ${PACKAGE_MAJOR_VERSION}
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES
    dictionary.h
    iniparser.h
    DESTINATION "include/iniparser")
