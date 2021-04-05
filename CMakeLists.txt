cmake_minimum_required (VERSION 3.10)

project (BSMath
	VERSION 0.0.1
	LANGUAGES CXX
	DESCRIPTION "Header only math library made by blAs1N"
	HOMEPAGE_URL "https://github.com/blAs1N/BSMath"
)

set (CMAKE_CXX_STANDARD 17)
set (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)

add_library (BSMath INTERFACE)
target_include_directories (${PROJECT_NAME} INTERFACE "Inc/")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package (Threads REQUIRED)
endif ()

include (CMake/GetGTest.cmake)

if (GTest_FOUND)
	enable_testing ()

message ("Include : ${GTEST_INCLUDE_DIR}")
message ("Main : ${GTEST_MAIN_LIBRARY}")
message ("Lib : ${GTEST_LIBRARY}")

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc ${GTEST_INCLUDE_DIR})
target_link_libraries(Tests PRIVATE BSMath GTest::GTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

enable_testing()
add_test(NAME Test COMMAND Tests)