cmake_minimum_required(VERSION 3.18)

project(
  iniparser
  DESCRIPTION "C library for parsing INI-style files"
  HOMEPAGE_URL https://gitlab.com/iniparser/iniparser/
  LANGUAGES C
  VERSION 4.2.4)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(
  BUILD_SHARED_LIBS
  "Build using shared libraries"
  ON)
# For packaging by tools like bitbake, shared and static libs should be build at
# once
cmake_dependent_option(
  BUILD_STATIC_LIBS
  "Build static libs"
  ON
  "BUILD_SHARED_LIBS"
  OFF)
if(BUILD_SHARED_LIBS)
  list(
    APPEND
    TARGET_TYPES
    "shared")
endif()
if(BUILD_STATIC_LIBS OR NOT BUILD_SHARED_LIBS)
  list(
    APPEND
    TARGET_TYPES
    "static")
endif()
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

foreach(TARGET_TYPE ${TARGET_TYPES})
  set(TARGET_NAME "${PROJECT_NAME}-${TARGET_TYPE}")
  set(TARGETS_EXPORT_NAME "${TARGET_NAME}Targets")
  string(TOUPPER ${TARGET_TYPE} UPPER_TARGET_TYPE)
  message(DEBUG "TARGET_TYPE: ${TARGET_TYPE}")
  message(DEBUG "UPPER_TARGET_TYPE: ${UPPER_TARGET_TYPE}")
  message(DEBUG "TARGET_NAME: ${TARGET_NAME}")
  add_library(
    ${TARGET_NAME}
    ${UPPER_TARGET_TYPE}
    "src/iniparser.c"
    "src/dictionary.c")
  set(PUBLIC_HEADERS "src/iniparser.h" "src/dictionary.h")
  set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
                                                  "${PUBLIC_HEADERS}")
  target_include_directories(
    ${TARGET_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)
  # If both shared and static libs are build at once with MSVC it generates a
  # shared library consisting of
  #
  # * a DLL (that contains the actual code) with filename suffix `.dll`,
  # * a correspoding import library (against which users should link) with
  #   filename suffix `.lib`.
  #
  # and a static library with filename suffix `.lib`
  #
  # As both, the shared import library and the static library share the same
  # basename and suffix one would overwrite the other.
  #
  # To solve this issue we set the PREFIX for *static* libs explicitely so MSVC
  # will build:
  #
  # * shared: iniparser.dll
  # * import: iniparser.lib
  # * static: libiniparser.lib
  #
  # As all other platforms already set `PREFIX` for all lib types they remain
  # unchanged. Therefore no `if(MSVC)` check is needed here.
  #
  if(TARGET_TYPE
     MATCHES
     "static")
    set_target_properties(${TARGET_NAME}
                          PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" PREFIX "lib")
  else()
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    # automatically create a module definition (.def) file for MSVC so it will
    # build an import library (.lib)
    set_property(TARGET ${TARGET_NAME} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS 1)
  endif()
  set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
  set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION
                                                  ${PROJECT_VERSION_MAJOR})

  if(UNIX)
    target_compile_options(
      ${TARGET_NAME}
      PRIVATE -fPIC
              -Wall
              -Wextra
              -pedantic)
  endif(UNIX)

  # install targets
  install(
    TARGETS ${TARGET_NAME}
    EXPORT ${TARGETS_EXPORT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # install DLLs on Windows
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

  # build directory package config
  export(
    TARGETS ${TARGET_NAME}
    APPEND
    FILE "${TARGETS_EXPORT_NAME}.cmake")

  # installed package config
  install(EXPORT ${TARGETS_EXPORT_NAME} DESTINATION "${CONFIG_INSTALL_DIR}")
endforeach()

# installed package config
set(PROJECT_CONFIG_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(PROJECT_VERSION_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/config.cmake.in ${PROJECT_CONFIG_FILE}
  INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")
# generate package version file
write_basic_package_version_file(
  ${PROJECT_VERSION_FILE}
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
# install version and config file
install(FILES ${PROJECT_VERSION_FILE} ${PROJECT_CONFIG_FILE}
        DESTINATION ${CONFIG_INSTALL_DIR})
file(
  GLOB
  FIND_MODULES
  "${PROJECT_SOURCE_DIR}/cmake/Find*.cmake")
message(DEBUG "Find modules: ${FIND_MODULES}")
install(FILES ${FIND_MODULES} DESTINATION ${CONFIG_INSTALL_DIR})
# make available in build directory for user package registry dependants
file(COPY ${FIND_MODULES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# add to CMake registry
export(PACKAGE ${PROJECT_NAME})
# generate pc-file
include(JoinPaths)
join_paths(
  libdir_for_pc_file
  "\${exec_prefix}"
  "${CMAKE_INSTALL_LIBDIR}")
join_paths(
  includedir_for_pc_file
  "\${prefix}"
  "${CMAKE_INSTALL_INCLUDEDIR}"
  "${PROJECT_NAME}")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  @ONLY)
# install pc-file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

option(BUILD_EXAMPLES "Build and install examples")
if(BUILD_EXAMPLES)
  add_executable(iniexample ${CMAKE_CURRENT_SOURCE_DIR}/example/iniexample.c)
  add_executable(iniwrite ${CMAKE_CURRENT_SOURCE_DIR}/example/iniwrite.c)
  add_executable(parse ${CMAKE_CURRENT_SOURCE_DIR}/example/parse.c)

  foreach(TARGET_TYPE ${TARGET_TYPES})
    # if BUILD_STATIC_LIBS=ON shared takes precedence
    target_link_libraries(iniexample ${PROJECT_NAME}-${TARGET_TYPE})
    target_link_libraries(iniwrite ${PROJECT_NAME}-${TARGET_TYPE})
    target_link_libraries(parse ${PROJECT_NAME}-${TARGET_TYPE})
  endforeach()
  install(
    TARGETS iniexample
            iniwrite
            parse
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/example/twisted.ini
          DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/)
endif()

option(
  BUILD_DOCS
  "Build and install docs"
  ON)
if(BUILD_DOCS)
  find_package(Doxygen REQUIRED)
  set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  doxygen_add_docs(
    docs
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/iniparser.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/iniparser.h
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()
