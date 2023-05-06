foreach (example
         "example"
         "example_cpp"
         "example_no_runner"
         "example_no_suite"
         "example_shuffle"
         "example_suite"
         "example_trunc")
    set(EXEC_NAME "${PROJECT_NAME}_${example}")
    if (example STREQUAL "example_cpp" OR example STREQUAL "example_suite")
        continue() #set(Source_Files "../${example}.cpp")
    elseif (example STREQUAL "example")
        set(Source_Files "../${example}.c" "../${example}_suite.c")
    else ()
        set(Source_Files "../${example}.c")
    endif ()
    source_group("Source Files" FILES "${Source_Files}")

    add_executable("${EXEC_NAME}" "${Header_Files}" "${Source_Files}")

    target_link_libraries("${EXEC_NAME}" PUBLIC "${PROJECT_NAME}_compiler_flags")

    include(GNUInstallDirs)
    target_include_directories(
            "${EXEC_NAME}"
            PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
    add_test(NAME "${EXEC_NAME}"
             COMMAND "${CMAKE_BINARY_DIR}/${EXEC_NAME}")
endforeach ()
