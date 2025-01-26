include(FetchContent)
include(CTest)

find_program(
  RUBY_EXECUTABLE
  NAME
  ruby
  PATHS ENV
        PATH
        REQUIRED)

if(MSVC)
  message(STATUS "MSVC environment")
  find_file(
    MSVC_DIRENT
    "dirent.h"
    REQUIRED)
  message(STATUS "MSVC_DIRENT: ${MSVC_DIRENT}")
  get_filename_component(
    MSVC_DIRENT_INCLUDE_DIR
    ${MSVC_DIRENT}
    DIRECTORY)
  message(STATUS "MSVC_DIRENT_INCLUDE_DIR: ${MSVC_DIRENT_INCLUDE_DIR}")
  include_directories(${MSVC_DIRENT_INCLUDE_DIR})
endif()

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  unity
  GIT_REPOSITORY "https://github.com/throwtheswitch/unity.git"
  GIT_PROGRESS TRUE
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_LIST_DIR}/unity_config.h ./src/)

FetchContent_MakeAvailable(unity)
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H
                                        UNITY_USE_COMMAND_LINE_ARGS)

function(create_test_runner)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs)
  cmake_parse_arguments(
    TEST_RUNNER
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN})

  message(STATUS "Creating test_${TEST_RUNNER_NAME}")
  add_custom_command(
    OUTPUT test_${TEST_RUNNER_NAME}_runner.c
    COMMAND
      ${RUBY_EXECUTABLE} ${unity_SOURCE_DIR}/auto/generate_test_runner.rb
      ${CMAKE_CURRENT_SOURCE_DIR}/test_${TEST_RUNNER_NAME}.c
      test_${TEST_RUNNER_NAME}_runner.c ${CMAKE_CURRENT_LIST_DIR}/unity-config.yml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test_${TEST_RUNNER_NAME}.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_executable(test_${TEST_RUNNER_NAME} test_${TEST_RUNNER_NAME}.c
                                          test_${TEST_RUNNER_NAME}_runner.c)
  foreach(TARGET_TYPE ${TARGET_TYPES})
    # if BUILD_STATIC_LIBS=ON shared takes precedence
    target_link_libraries(
      test_${TEST_RUNNER_NAME}
      ${PROJECT_NAME}-${TARGET_TYPE}
      unity)
  endforeach()
endfunction()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ressources
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
create_test_runner(NAME ${PROJECT_NAME})
message(STATUS "Adding test test_${PROJECT_NAME}")
add_test(
  NAME test_${PROJECT_NAME}
  COMMAND test_${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
  TARGET test_${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:test_${PROJECT_NAME}>
          $<TARGET_RUNTIME_DLLS:test_${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS)

create_test_runner(NAME dictionary)
message(STATUS "Adding test test_dictionary")
add_test(
  NAME test_dictionary
  COMMAND test_dictionary
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
  TARGET test_dictionary
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:test_dictionary>
          $<TARGET_RUNTIME_DLLS:test_dictionary>
  COMMAND_EXPAND_LISTS)
