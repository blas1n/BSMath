if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package (Threads REQUIRED)
endif ()

enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_FILES "*.cpp")
add_executable(Tests ${TEST_FILES})

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc ${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests PRIVATE BSMath GTest::GTest)

add_test(AllTests Tests)