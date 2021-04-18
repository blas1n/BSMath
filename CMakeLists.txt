cmake_minimum_required (VERSION 3.12)

project (BSMath
	VERSION 0.1.0
	LANGUAGES CXX
	DESCRIPTION "Header only math library made by blAs1N"
	HOMEPAGE_URL "https://github.com/blAs1N/BSMath"
)

set (CMAKE_CXX_STANDARD 17)
set (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)

add_subdirectory (ThirdParty)

add_library (BSMath INTERFACE)
target_link_libraries (BSMath INTERFACE BSBase)
target_include_directories (${PROJECT_NAME} INTERFACE "Inc/")

add_subdirectory (Tests)