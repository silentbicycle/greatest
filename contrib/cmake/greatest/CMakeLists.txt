set(Header_Files "${CMAKE_SOURCE_DIR}/greatest.h")
source_group("Header Files" FILES "${Header_Files}")

add_library("${PROJECT_NAME}" INTERFACE "${Header_Files}")

target_link_libraries("${PROJECT_NAME}" INTERFACE "${PROJECT_NAME}_compiler_flags")

include(GNUInstallDirs)

#=========#
# Install #
#=========#

# setup the version numbering
set_property(TARGET "${PROJECT_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

install(FILES ${Header_Files}
        TYPE "INCLUDE")

#====================#
# Package to install #
#====================#

set(installable_libs "${PROJECT_NAME}" "${PROJECT_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
    list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(TARGETS ${installable_libs}
        EXPORT "${PROJECT_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(EXPORT "${PROJECT_NAME}Targets" DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
