cmake_minimum_required(VERSION 3.20)

project(hydrogen)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
	src/main.cpp
)

add_executable(hydro ${SOURCES})