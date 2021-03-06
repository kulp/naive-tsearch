cmake_minimum_required(VERSION 3.10)

set(NAIVE_TSEARCH_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(NAIVE_TSEARCH_MAIN_PROJECT ON)
endif()

file(READ "NAIVETSEARCH_VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)

project(naive-tsearch VERSION "${VERSION}" LANGUAGES C
    DESCRIPTION "A simple tsearch() implementation for platforms without one")

# Using HOMEPAGE_URL in `project` requires CMake >=3.12
set(PROJECT_HOMEPAGE_URL "https://github.com/kulp/naive-tsearch")

if(MSVC)
    # Remove warning flags to avoid `cl : Command line warning D9025 : overriding '/W3' with '/W4'` warning
    string(REGEX REPLACE "/W[0-9]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(TARGETS)
endif()

include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(GNUInstallDirs)

add_library(naive-tsearch STATIC tsearch.c)
add_library(naive-tsearch::naive-tsearch ALIAS naive-tsearch)
target_include_directories(naive-tsearch PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}")
set(NAIVE_TSEARCH_PUBLIC_HEADERS
    search.h
    tsearch.h
    tsearch.h.inc)
set_target_properties(naive-tsearch PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF)
target_compile_options(naive-tsearch PRIVATE
    $<IF:$<C_COMPILER_ID:MSVC>,/W4,-Wall -Wextra>)

add_library(naive-tsearch-hdronly INTERFACE)
add_library(naive-tsearch::naive-tsearch-hdronly ALIAS naive-tsearch-hdronly)
target_include_directories(naive-tsearch-hdronly INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}")
target_compile_definitions(naive-tsearch-hdronly INTERFACE
    NAIVE_TSEARCH_HDRONLY)
set(NAIVE_TSEARCH_HDRONLY_PUBLIC_HEADERS
    tsearch_hdronly.h
    tsearch.c.inc
    tsearch.h.inc)

option(NAIVE_TSEARCH_TESTS "Build ${PROJECT_NAME} tests + enable CTest" "${NAIVE_TSEARCH_MAIN_PROJECT}")
if(NAIVE_TSEARCH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

option(NAIVE_TSEARCH_INSTALL "Enable ${PROJECT_NAME} install" "${NAIVE_TSEARCH_MAIN_PROJECT}")
if(NAIVE_TSEARCH_INSTALL)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(naive-tsearchConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion)

    configure_file(naive-tsearch.pc.in naive-tsearch.pc @ONLY)

    install(TARGETS naive-tsearch EXPORT naive-tsearchTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${NAIVE_TSEARCH_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    install(TARGETS naive-tsearch-hdronly EXPORT naive-tsearchTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${NAIVE_TSEARCH_HDRONLY_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    install(EXPORT naive-tsearchTargets
        FILE naive-tsearchConfig.cmake
        NAMESPACE naive-tsearch::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    install(FILES ${PROJECT_BINARY_DIR}/naive-tsearchConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/naive-tsearch.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME})

    set(CPACK_GENERATOR "TGZ;TXZ")
    include(CPack)
endif()
