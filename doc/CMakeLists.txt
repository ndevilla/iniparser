configure_file(${CMAKE_CURRENT_SOURCE_DIR}/iniparser.dox.in ${CMAKE_CURRENT_BINARY_DIR}/iniparser.dox @ONLY)

add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/iniparser.dox
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
	COMMENT "Generating API documentation with Doxygen" VERBATIM
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION share/doc/Iniparser)
