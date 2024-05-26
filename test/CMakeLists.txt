include(FetchContent)
include(CTest)
find_package(
  Ruby
  2.0
  REQUIRED)

FetchContent_Declare(
  unity
  GIT_REPOSITORY "https://github.com/throwtheswitch/unity.git"
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_LIST_DIR}/unity_config.h ./src/)

FetchContent_MakeAvailable(unity)
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H)

add_custom_command(
  OUTPUT test_runner.c
  COMMAND ${RUBY_EXECUTABLE} ${unity_SOURCE_DIR}/auto/generate_test_runner.rb
          ${CMAKE_CURRENT_SOURCE_DIR}/test.c test_runner.c
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  BYPRODUCTS test_runner.c)

add_executable(test_iniparser test.c test_runner.c)
foreach(TARGET_TYPE ${TARGET_TYPES})
  # if BUILD_STATIC_LIBS=ON shared takes precedence
  target_link_libraries(
    test_iniparser
    iniparser-${TARGET_TYPE}
    unity)
endforeach()
enable_testing()
add_test(
  NAME test_iniparser
  COMMAND test_iniparser
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS test_iniparser RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ressources
        DESTINATION ${CMAKE_INSTALL_BINDIR})
