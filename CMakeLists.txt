cmake_minimum_required (VERSION 3.8)

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

enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_FILES "Tests/*.cpp")
add_executable(Tests ${TEST_FILES})

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc ${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests PRIVATE BSMath GTest::GTest)

add_test(AllTests Tests)