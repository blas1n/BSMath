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

find_package(GTest)

if (GTest_FOUND)
	file(GLOB_RECURSE TEST_FILES "Tests/*.cpp")
	add_executable(Tests ${TEST_FILES})

	target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc)
	target_link_libraries(Tests PRIVATE BSMath GTest::GTest)

	enable_testing()
	add_test(NAME Test COMMAND Tests)
endif ()