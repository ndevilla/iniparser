file(GLOB exfiles *.c)
file(GLOB inifiles *.ini)
file(COPY ${inifiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Construct Example Target Target
foreach(it ${exfiles})
	## Construct Example Executables
	get_filename_component(exname ${it} NAME_WE)
	add_executable(${exname} ${it})
	target_link_libraries(${exname} Iniparser)

	## properties
	set_property(TARGET ${exname} PROPERTY C_STANDARD 11)
	set_property(TARGET ${exname} PROPERTY POSITION_INDEPENDENT_CODE ON)
endforeach(it)
