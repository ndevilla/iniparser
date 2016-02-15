file(GLOB srcs *.c)
file(GLOB hdrs *.h)

## Library Target
add_library(iniparser ${srcs})
set_property(TARGET iniparser PROPERTY C_STANDARD 11)
set_property(TARGET iniparser PROPERTY POSITION_INDEPENDENT_CODE ON)

## Install Target
install(FILES ${hdrs} DESTINATION include)
install(TARGETS iniparser
	LIBRARY DESTINATION lib	)