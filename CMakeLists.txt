cmake_minimum_required(VERSION 3.11)

project(greatest VERSION 1.5.0 LANGUAGES C)

#===============#
# Compiler args #
#===============#

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
set(CMAKE_C_STANDARD 90 CACHE STRING "")

add_library("${PROJECT_NAME}_compiler_flags" INTERFACE)
if ((NOT DEFINED MSVC_VERSION
        OR MSVC_VERSION STRGREATER "1900") # 2015
        AND NOT (CMAKE_C_COMPILER_ID STREQUAL "OpenWatcom"))
    target_compile_features("${PROJECT_NAME}_compiler_flags" INTERFACE "c_std_${CMAKE_C_STANDARD}")
else ()
    message(STATUS "not adding compile features")
endif ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15")
    # add compiler warning flags just when building this project via
    # the BUILD_INTERFACE genex
    set(gcc_like "$<COMPILE_LANG_AND_ID:C,CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
    set(msvc "$<COMPILE_LANG_AND_ID:C,CXX,MSVC>")
    target_compile_options(
            "${PROJECT_NAME}_compiler_flags"
            INTERFACE
            "$<${gcc_like}:$<BUILD_INTERFACE:-Wshadow;-Wformat=2;-Wall;-pedantic;-Wno-format-nonliteral;-Wno-unused-function;-Wno-long-long>>"
            "$<${msvc}:$<BUILD_INTERFACE:-W3;-WX;-Zi;-permissive->>"
    )
endif (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15")

# Set the build directories
if (CMAKE_SYSTEM_NAME STREQUAL "Windows"
        OR CMAKE_SYSTEM_NAME STREQUAL "CYGWIN"
        OR CMAKE_SYSTEM_NAME MATCHES "MINGW.*")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
else ()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif ()

#===========#
# Configure #
#===========#

# configure a header file to pass the version number only
configure_file(
        "${CMAKE_SOURCE_DIR}/contrib/cmake/config.h.in"
        "${PROJECT_NAME}Config.h"
)

#=============#
# Sub-targets #
#=============#

include(CTest)
if (BUILD_TESTING)
    add_subdirectory("${CMAKE_SOURCE_DIR}/contrib/cmake/example")
    add_subdirectory("${CMAKE_SOURCE_DIR}/contrib/cmake/example_cpp")
endif (BUILD_TESTING)

#=========#
# Install #
#=========#

include(GNUInstallDirs)

include(InstallRequiredSystemLibraries)
set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "SamuelMarks")
set(CPACK_PACKAGE_DESCRIPTION "A C testing library in 1 file. No dependencies, no dynamic allocation. ISC licensed.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
if (APPLE)
    set(CPACK_BUNDLE_PLIST "contrib/cmake/Info.plist")
    set(CPACK_BUNDLE_ICON "contrib/cmake/Info.plist")
    set(CPACK_PACKAGE_ICON "contrib/cmake/CustomVolumeIcon.icns")
endif (APPLE)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/contrib/cmake/README.txt")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/contrib/cmake/Welcome.txt")
set(CPACK_PACKAGE_CONTACT "https://github.com/silentbicycle/${PROJECT_NAME}")

include(CPack)
include(CMakePackageConfigHelpers)

# generate the config file that is includes the exports
configure_package_config_file(
        "contrib/cmake/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
