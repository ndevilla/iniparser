file(GLOB tstfiles *.c)
file(GLOB inifiles *.ini)

# Copy File To Binary file
file(COPY ressources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY CuTest_license.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY make-tests.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Construct Example Target Target
foreach(it ${tstfiles})
	## Construct Example Executables
	get_filename_component(tstname ${it} NAME_WE)
	add_executable(${tstname} ${it})
	target_link_libraries(${tstname} iniparser)

	## properties
	set_property(TARGET ${tstname} PROPERTY C_STANDARD 11)
	set_property(TARGET ${tstname} PROPERTY POSITION_INDEPENDENT_CODE ON)
endforeach(it)
