cmake_minimum_required(VERSION 3.20)
project(KittySWF VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(SDL_X11_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(external/SDL2)

add_subdirectory(external/zlib)

file(GLOB_RECURSE PLAYER_SOURCES src/*.cpp)
list(APPEND PLAYER_SOURCES external/glad/src/glad.c)


add_executable(KittySWF ${PLAYER_SOURCES})

target_include_directories(KittySWF PRIVATE
	src/
	external/stb
	external/miniaudio
	external/glad/include
	external/zlib
)

target_link_libraries(KittySWF PRIVATE
	SDL3-static
	zlibstatic
	xcb
	X11
)

if(MSVC)
	set_property(TARGET KittySWF PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded")
elseif(MINGW)
	target_link_options(KittySWF PRIVATE
		-static
		-static-libgcc
		-static-libstdc++)

elseif(UNIX AND NOT APPLE)
	target_link_options(KittySWF PRIVATE
		-static-libgcc
		-static-libstdc++)
endif()


