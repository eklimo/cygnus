cmake_minimum_required(VERSION 3.1)
project(cygnus LANGUAGES CXX)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
	  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/cli.cpp"
)

# core files
add_library(cygnus-core SHARED ${SOURCES})
target_include_directories(cygnus-core PUBLIC ${SRC_DIR})
target_compile_features(cygnus-core PUBLIC cxx_std_17)
target_compile_options(cygnus-core PRIVATE -Wall)

# main executable
add_executable(cygnus "${SRC_DIR}/main.cpp" "${SRC_DIR}/cli.cpp")
set_target_properties(cygnus PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_compile_options(cygnus PRIVATE -Wall)
target_link_libraries(cygnus cygnus-core)

# tests
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE test)

add_executable(test test/main.cpp)
set_target_properties(test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_compile_options(test PRIVATE -Wall)
target_link_libraries(test doctest cygnus-core)
