cmake_minimum_required(VERSION 3.12)

project(iniparser VERSION 4.2.1)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(INIPARSER_SRCS src/dictionary.c src/iniparser.c)
set(INIPARSER_HDRS src/dictionary.h src/iniparser.h)

add_library(iniparser-shared SHARED ${INIPARSER_SRCS} ${INIPARSER_HDRS})
add_library(iniparser-static STATIC ${INIPARSER_SRCS} ${INIPARSER_HDRS})

target_include_directories(
  iniparser-shared
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)
target_include_directories(
  iniparser-static
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)

set_target_properties(iniparser-shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(iniparser-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(iniparser-shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(iniparser-static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(iniparser-shared PROPERTIES SOVERSION
                                                  ${PROJECT_VERSION_MAJOR})

if(UNIX)
  target_compile_options(
    iniparser-shared
    PRIVATE -fPIC
            -Wall
            -Wextra
            -pedantic)
  target_compile_options(
    iniparser-static
    PRIVATE -fPIC
            -Wall
            -Wextra
            -pedantic)
endif(UNIX)

# install targets
install(
  TARGETS iniparser-shared
  EXPORT iniparser-sharedTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(
  TARGETS iniparser-static
  EXPORT iniparser-staticTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# build directory package config
export(
  TARGETS iniparser-shared
  APPEND
  FILE "iniparser-sharedTargets.cmake")
export(
  TARGETS iniparser-static
  APPEND
  FILE "iniparser-staticTargets.cmake")

install(EXPORT iniparser-sharedTargets DESTINATION "${CONFIG_INSTALL_DIR}")
install(EXPORT iniparser-staticTargets DESTINATION "${CONFIG_INSTALL_DIR}")

# installed package config
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
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

enable_testing()

add_test(
  NAME testsuite
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
