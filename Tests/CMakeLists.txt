file(GLOB_RECURSE TEST_FILES "*.cpp")
add_executable(Tests ${TEST_FILES})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package (Threads REQUIRED)
endif ()

find_package(GTest REQUIRED)
enable_testing()
add_test(NAME "Tests" COMMAND Tests)

include_directories(${PROJECT_SOURCE_DIR}/Inc ${GTEST_INCLUDE_DIRS})
link_libraries(BSMath GTest::GTest)