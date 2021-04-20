cmake_minimum_required (VERSION 3.12)

project (BSMath
	VERSION 0.1.0
	LANGUAGES CXX
	DESCRIPTION "Header only math library made by blAs1N"
	HOMEPAGE_URL "https://github.com/blAs1N/BSMath"
)

include (GNUInstallDirs)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)

add_library (${PROJECT_NAME} INTERFACE)
add_library (${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package (BSBase CONFIG REQUIRED)
target_link_libraries (BSMath INTERFACE BSBase::BSBase)

target_include_directories (${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Inc>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include (CMake/InstallProject.cmake)
add_subdirectory (Tests)