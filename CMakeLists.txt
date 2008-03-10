# (c) 2007-2008 David Roberts

PROJECT(cube)

SET(SOURCES
	softlines.cpp
	cube.cpp
)

FIND_PACKAGE(SDL REQUIRED)

LINK_LIBRARIES(
	${SDL_LIBRARY}
)

ADD_EXECUTABLE(cube
	WIN32 # hide DOS prompt under Windows
	MACOSX_BUNDLE
	${SOURCES}
)
