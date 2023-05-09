set(ORIG_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
cmake_minimum_required(VERSION 3.11)
project(greatest_cpp VERSION 1.5.0 LANGUAGES CXX)
set(EXEC_NAME "${PROJECT_NAME}")
set(Header_Files "${CMAKE_SOURCE_DIR}/greatest.h")
source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "${CMAKE_SOURCE_DIR}/example_cpp.cpp")
add_executable("${EXEC_NAME}" "${Source_Files}")

include(GNUInstallDirs)
target_include_directories(
        "${EXEC_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${ORIG_CMAKE_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
add_test(NAME "${EXEC_NAME}"
        COMMAND "${CMAKE_BINARY_DIR}/${EXEC_NAME}")
