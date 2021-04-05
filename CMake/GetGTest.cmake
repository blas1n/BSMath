function (download_gtest)
    set (SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/GTest/GTest-Src")
    set (BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/GTest/GTest-Build")
    set (DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/GTest/GTest-Download")
    configure_file ("${PROJECT_SOURCE_DIR}/CMake/GTest.cmake" "${DOWNLOAD_DIR}/CMakeLists.txt" @ONLY)

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
endfunction ()

find_package (GTest)

if (NOT GTest_FOUND)
    download_gtest ()
endif ()