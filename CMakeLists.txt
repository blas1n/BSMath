cmake_minimum_required (VERSION 3.8)

project (BSMath
	VERSION 0.0.1
	LANGUAGES CXX
	DESCRIPTION "Header only math library made by blAs1N"
	HOMEPAGE_URL "https://github.com/blAs1N/BSMath"
)

add_library(BSMath INTERFACE)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE "Inc/")

add_subdirectory(Tests)