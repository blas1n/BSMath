cmake_minimum_required (VERSION 3.8)

project (BSMath
	VERSION 0.0.1
	LANGUAGES CXX
	DESCRIPTION "Header only math library made by blAs1N"
	HOMEPAGE_URL "https://github.com/blAs1N/BSMath"
)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE "include")

file(GLOB_RECURSE TEST_FILES "Tests/*.cpp")
add_executable(Tests ${TEST_FILES})
set_target_properties(Tests PROPERTIES CXX_STANDARD 17)

find_package(GTest REQUIRED)
enable_testing()
add_test(NAME "Tests" COMMAND Tests)

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc)
target_link_libraries(Tests PRIVATE ${CMAKE_PROJECT_NAME} GTest::GTest)