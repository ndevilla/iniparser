CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(iniparser C)

## Version
set(INIPARSER_MAJOR_VERSION 4)
set(INIPARSER_MINOR_VERSION 0)
set(INIPARSER_PATCH_VERSION 0)
set(INIPARSER_VERSION
  ${INIPARSER_MAJOR_VERSION}.${INIPARSER_MINOR_VERSION}.${INIPARSER_PATCH_VERSION})
 
## CMAKE CONF
set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS ON)

## OSX Flags
if(CMAKE_HOST_APPLE)
	set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif(CMAKE_HOST_APPLE)

## Build libiniparser.so/dylib
include_directories(src)
add_subdirectory(src)

## Doc target
find_package(Doxygen)
if (DOXYGEN_FOUND)
	file(COPY html DESTINATION ${CMAKE_BINARY_DIR})
	add_subdirectory(doc)
endif (DOXYGEN_FOUND)

## Examples
set(BUILD_EXAMPLES ON CACHE BOOL "Build iniparser examples")
if(BUILD_EXAMPLES)
	add_subdirectory(example)
endif(BUILD_EXAMPLES)

## Tests ## Needed To Be Fixed
set(BUILD_TESTS OFF CACHE BOOL "Build Tests")
if(BUILD_TESTS)
	add_subdirectory(test)
endif(BUILD_TESTS)

## Config.cmake -> so find_package(Iniparser works)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)

include(CMakePackageConfigHelpers)
configure_package_config_file(IniparserConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/IniparserConfig.cmake
                              INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
                              PATH_VARS INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/IniparserConfigVersion.cmake
                                 VERSION ${INIPARSER_VERSION} 
                                 COMPATIBILITY AnyNewerVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/IniparserConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/IniparserConfigVersion.cmake
        DESTINATION ${LIBRARY_INSTALL_DIR}/cmake/Iniparser )