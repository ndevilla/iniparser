## Sources 
file(GLOB srcs *.c)

## Library Target
add_library(Iniparser ${srcs})

## C Property
set_property(TARGET Iniparser PROPERTY C_STANDARD 11)
set_property(TARGET Iniparser PROPERTY POSITION_INDEPENDENT_CODE ON)

## Version Property
set_property(TARGET Iniparser PROPERTY VERSION ${INIPARSER_VERSION})
set_property(TARGET Iniparser PROPERTY SOVERSION ${INIPARSER_MAJOR_VERSION})
set_property(TARGET Iniparser PROPERTY
					INTERFACE_INIPARSER_MAJOR_VERSION 3)
set_property(TARGET Iniparser APPEND PROPERTY
					COMPATIBLE_INTERFACE_STRING INIPARSER_MAJOR_VERSION)

## IniparserConfig.cmake
set(include_install_dir "include")
set(library_install_dir "lib")
set(config_install_dir "${library_install_dir}/cmake/Iniparser")

## Generated
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(version_config "${generated_dir}/IniparserConfigVersion.cmake")
set(project_config "${generated_dir}/IniparserConfig.cmake")
set(target_export_name "IniparserTargets")

## Config.cmake marcros
include(CMakePackageConfigHelpers)

## Versionning
write_basic_package_version_file(
	${version_config} VERSION ${INIPARSER_VERSION} COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	${CMAKE_SOURCE_DIR}/IniparserConfig.cmake.in 
	${project_config}
	INSTALL_DESTINATION ${config_install_dir}
	PATH_VARS include_install_dir library_install_dir
	# NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

## Install Project
install(
	TARGETS Iniparser
	EXPORT ${target_export_name}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include
)

## headers
file(GLOB hdrs *.h)
install(FILES ${hdrs} DESTINATION include)

## Export
install(FILES ${project_config} ${version_config}
		DESTINATION ${config_install_dir})
install(EXPORT ${target_export_name}
		DESTINATION ${config_install_dir})