cmake_minimum_required(VERSION 3.10)
project(SAGE_v2)

set(CMAKE_CXX_STANDARD 17)

# Add the executable
add_executable(SAGE 
	src/SAGE.cpp
	src/SAGE_Engine.cpp
	src/Parser.cpp
	src/Logger.cpp
	src/DataFrame.cpp
	src/Comparer.cpp
	src/Archiver.cpp
	src/external/pugixml.cpp
	src/external/miniz.c)

# Include headers from the root and external folder
target_include_directories(SAGE PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/external
)