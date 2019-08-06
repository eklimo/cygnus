cmake_minimum_required(VERSION 3.1)
project(cygnus LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(cygnus ${SOURCES})

target_include_directories(cygnus
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(cygnus PRIVATE -Wall)
target_compile_features(cygnus PRIVATE cxx_std_17)

set_target_properties(cygnus PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
