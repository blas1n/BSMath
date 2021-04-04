function (download_gtest)
    set (CMAKE_DIR "${PROJECT_SOURCE_DIR}/CMake")
    set (SRC_DIR "${PROJECT_SOURCE_DIR}/GTest")
    set (BUILD_DIR "${CMAKE_BINARY_DIR}/GTest/GTest-Build")
    set (DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/GTest/GTest-Download")

    configure_file ("${CMAKE_DIR}/GTest.cmake" "${DOWNLOAD_DIR}/CMakeLists.txt" @ONLY)

    execute_process (
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE RESULT
        WORKING_DIRECTORY "${DOWNLOAD_DIR}"
    )

    if (RESULT)
        message (FATAL_ERROR "CMake step for GTest failed: ${RESULT}")
    endif ()

    execute_process (
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE RESULT
        WORKING_DIRECTORY "${DOWNLOAD_DIR}"
    )

    if (RESULT)
        message (FATAL_ERROR "Build step for GTest failed: ${RESULT}")
    endif ()

    if (EXISTS "${SRC_DIR}/CMakeLists.txt")
         add_subdirectory ("${SRC_DIR}" "${BUILD_DIR}" EXCLUDE_FROM_ALL)
    endif ()
endfunction ()

function (get_gtest)
    find_package (GTest CONFIG QUIET)

    if (GTest_FOUND)
        message (STATUS "Found GTest from system")
    else ()
        message (STATUS "Could not find GTest from system. Downloading...")
        download_gtest ()
    endif ()
endfunction ()
