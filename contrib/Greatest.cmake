#[=======================================================================[.rst
Greatest
--------

This module defines a function to help use Greatest with CMake.
:command:`greatest_add_tests` scans source files to identify tests. It is
intended to replace use of :command:`add_test` to register tests, and will
create a separate CTest test for each Greatest test case. Note that this is in
some cases less efficient, as common set-up and tear-down logic cannot be
shared by multiple test cases executing in the same instance. However, it
provides more fine-grained pass/fail information to CTest, which is usually
considered as more beneficial.  By default, the CTest test name is the same as
the Greatest name; see also ``TEST_PREFIX`` and ``TEST_SUFFIX``.

.. command:: greatest_add_tests

  Automatically add tests with CTest by scanning source code for Greatest
  macros::

    greatest_add_tests(TARGET target
                       [SOURCES src1...]
                       [EXTRA_ARGS arg1...]
                       [WORKING_DIRECTORY dir]
                       [TEST_PREFIX prefix]
                       [TEST_SUFFIX suffix]
                       [SKIP_DEPENDENCY]
                       [TEST_LIST outVar]
    )

  ``greatest_add_tests`` attempts to identify tests by scanning source files.

  The options are:

  ``TARGET target``
    Specifies the Greatest executable, which must be a known CMake executable
    target.  CMake will substitute teh location of the built executable when
    running the test.

  ``SOURCES src1..``
    When provided, only the listed files will be scanned for test cases.  If
    this option is not given, the :prop_tgt:`SOURCES` property of the specified
    ``target`` will be used to obtain the list of sources.

  ``EXTRA_ARGS arg1..``
    Any extra arguments to pass on the command line to each test case.

  ``WORKING_DIRECTORY dir``
    Specifies the directory in which to run the discovered test cases.  If this
    option is not provided, the current binary directory is used.

  ``TEST_PREFIX prefix``
    Specifies a ``prefix`` to be prepended to the name of each discovered test
    case.  This can be useful when the same source files are being used in
    multiple calls to ``greatest_add_tests()`` but with different
    ``EXTRA_ARGS``.

  ``TEST_SUFFIX suffix``
    Similar to ``TEST_PREFIX`` except the ``suffix`` is appended to the name of
    every discovered test case.  Both ``TEST_PREFIX`` and ``TEST_SUFFIX`` may
    be specified.

  ``SKIP_DEPENDENCY``
    Normally, the function creates a dependency which will cause CMake to be
    re-run if any of the sources being scanned are changed.  This is to ensure
    that the list of discovered tests is updated.  If this behavior is not
    desired (as may be the case while actually writing the test cases), this
    option can be used to prevent the dependency from being added.

  ``TEST_LIST outVar``
    The variable named by ``outVar`` will be populated in the calling scope
    with the list of discovered test cases.  This allows the caller to do
    things like manipulate test properties of the discovered tests.

  Usage example:

  .. code-block:: cmake

    include( Greatest )
    add_executable( FooTest FooUnitTest.c )
    greatest_add_tests( TARGET      FooTest
                        TEST_SUFFIX .noArgs
                        TEST_LIST   noArgsTests
    )
    greatest_add_tests( TARGET      FooTest
                        EXTRA_ARGS  --someArg someValue
                        TEST_SUFFIX .withArgs
                        TEST_LIST   withArgsTests
                        )
    set_tests_properties( ${noArgsTests}   PROPERTIES TIMEOUT 10 )
    set_tests_properties( ${withArgsTests} PROPERTIES TIMEOUT 20 )

#]=======================================================================]


#------------------------------------------------------------------------------
function( greatest_add_tests )

    if( ARGC LESS 1 )
        message( FATAL_ERROR "No arguments supplied to greatest_add_tests()" )
    endif()

    set( options
        SKIP_DEPENDENCY
    )
    set( oneValueArgs
        TARGET
        WORKING_DIRECTORY
        TEST_PREFIX
        TEST_SUFFIX
        TEST_LIST
    )
    set( multiValueArgs
        SOURCES
        EXTRA_ARGS
    )
    set( allKeywords ${options} ${oneValueArgs} ${multiValueArgs} )

    unset( sources )

    cmake_parse_arguments( ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    set( autoAddSources YES )

    if( NOT TARGET "${ARGS_TARGET}" )
        message( FATAL_ERROR "${ARGS_TARGET} does not define an existing CMake target" )
    endif()

    if( NOT ARGS_WORKING_DIRECTORY )
        unset( workDir )
    else()
        set( workDir WORKING_DIRECTORY "${ARGS_WORKING_DIRECTORY}" )
    endif()

    if( NOT ARGS_SOURCES )
        get_property( ARGS_SOURCES TARGET ${ARGS_TARGET} PROPERTY SOURCES )
    endif()

    unset( testList )

    set( greatest_case_name_regex "TEST +[A-Za-z_0-9]+" )

    foreach( source IN LISTS ARGS_SOURCES )
        if( NOT ARGS_SKIP_DEPENDENCY )
            set_property( DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${source} )
        endif()
        file( READ "${source}" contents )
        string( REGEX MATCHALL "${greatest_case_name_regex}" found_tests "${contents}" )
        foreach( hit ${found_tests} )
            string( REGEX REPLACE "TEST +" "" greatest_test_name ${hit} )
            set( ctest_test_name ${ARGS_TEST_PREFIX}${greatest_test_name}${ARGS_TEST_SUFFIX} )
            add_test( NAME ${ctest_test_name}
                ${workDir}
                COMMAND ${ARGS_TARGET}
                  -t ${greatest_test_name}
                  ${ARGS_EXTRA_ARGS}
            )
            list( APPEND testList ${ctest_test_name} )
        endforeach()
    endforeach()

    if( ARGS_TEST_LIST )
        set( ${ARGS_TEST_LIST} ${testList} PARENT_SCOPE )
    endif()

endfunction()
