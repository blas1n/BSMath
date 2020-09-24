file(GLOB_RECURSE TEST_FILES "*.cpp")
add_executable(Tests ${TEST_FILES})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package (Threads REQUIRED)
endif ()

find_package(GTest REQUIRED)
enable_testing()
add_test(NAME "Tests" COMMAND Tests)

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/Inc)
target_link_libraries(Tests PRIVATE BSMath GTest::GTest)