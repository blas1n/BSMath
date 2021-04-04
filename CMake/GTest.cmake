cmake_minimum_required (VERSION 3.10)
project (GTest NONE)
include (ExternalProject)

ExternalProject_Add (GTest
	GIT_REPOSITORY		https://github.com/google/googletest.git
	GIT_TAG				release-1.10.0
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			${SRC_DIR}
	BINARY_DIR			${BUILD_DIR}
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)