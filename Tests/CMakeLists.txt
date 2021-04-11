# Tests

cmake_minimum_required (VERSION 3.10)

project (BSMath-Tests
	VERSION 0.1.0
	LANGUAGES CXX
)

if (GTest_FOUND)
	file(GLOB_RECURSE TEST_FILES "*.cpp")
	add_executable(BSMath-Tests ${TEST_FILES})
	target_link_libraries(BSMath-Tests PRIVATE BSMath GTest::GTest)

	enable_testing()
	add_test(NAME BSMath-Test COMMAND BSMath-Tests)
endif ()