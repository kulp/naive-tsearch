cmake_minimum_required(VERSION 3.8)

project(naive-tsearch VERSION 0.1 LANGUAGES C
    HOMEPAGE_URL "https://github.com/kulp/naive-tsearch"
    DESCRIPTION "A simple tsearch() implementation for platforms without one")

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()
endif()

include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(GNUInstallDirs)

check_include_file(search.h HAS_INCLUDE_SEARCH_H)
if(HAS_INCLUDE_SEARCH_H)
    check_c_source_compiles("#include <search.h>\nint main(){VISIT v = leaf;}" HAS_ENUM_VISIT)
    check_c_source_compiles("#include <search.h>\nint main(){tsearch(NULL, NULL, NULL);}" HAS_FUNCTION_TSEARCH)
    check_c_source_compiles("#include <search.h>\nint main(){tfind(NULL, NULL, NULL);}" HAS_FUNCTION_TFIND)
    check_c_source_compiles("#include <search.h>\nint main(){tdelete(NULL, NULL, NULL);}" HAS_FUNCTION_TDELETE)
    check_c_source_compiles("#include <search.h>\nint main(){tdelete(NULL, NULL);}" HAS_FUNCTION_TWALK)
endif()
configure_file(tsearch_cfg.h.in tsearch_cfg.h @ONLY)

add_library(naive-tsearch STATIC tsearch.c)
add_library(naive-tsearch::naive-tsearch ALIAS naive-tsearch)
target_include_directories(naive-tsearch PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
set_target_properties(naive-tsearch PROPERTIES
        PUBLIC_HEADER "tsearch.h;${PROJECT_BINARY_DIR}/tsearch_cfg.h"
        C_STANDARD 99
        C_EXTENSIONS OFF)
target_compile_definitions(naive-tsearch PRIVATE
    "$<IF:$<C_COMPILER_ID:MSVC>,/W4,-Wall -Wextra>")

option(NAIVE_TSEARCH_TESTS "Build ${PROJECT_NAME} tests + enable CTest")
if(NAIVE_TSEARCH_TESTS)
    add_executable(test_tsearch test_tsearch.c)
    target_link_libraries(test_tsearch PUBLIC naive-tsearch)
    target_compile_definitions(test_tsearch PRIVATE
        "$<IF:$<C_COMPILER_ID:MSVC>,/W4,-Wall -Wextra>")
    enable_testing()
    add_test(NAME test_tsearch COMMAND test_tsearch)
endif()

option(NAIVE_TSEARCH_INSTALL "Enable ${PROJECT_NAME} install")
if(NAIVE_TSEARCH_INSTALL)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(naive-tsearchConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion)

    configure_file(naive-tsearch.pc.in naive-tsearch.pc @ONLY)

    install(TARGETS naive-tsearch EXPORT naive-tsearchTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/naive-tsearch
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(EXPORT naive-tsearchTargets
          FILE naive-tsearchConfig.cmake
          NAMESPACE naive-tsearch::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/naive-tsearch)
    install(FILES ${PROJECT_BINARY_DIR}/naive-tsearchConfigVersion.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/naive-tsearch)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/naive-tsearch.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/naive-tsearch)

    if(EXISTS "${PROJECT_SOURCE_DIR}/.gitignore")
        # Simple glob to regex conversion (.gitignore => CPACK_SOURCE_IGNORE_FILES)
        file(READ ".gitignore" DOT_GITIGNORE)
        string(REPLACE ";" "RANDOMSEQUENCE" DOT_GITIGNORE "${DOT_GITIGNORE}")
        string(REPLACE "\n" ";" DOT_GITIGNORE "${DOT_GITIGNORE}")
        string(REPLACE "RANDOMSEQUENCE" "\\;" DOT_GITIGNORE "${DOT_GITIGNORE}")
        foreach(IGNORE_LINE ${DOT_GITIGNORE})
            if(NOT IGNORE_LINE OR IGNORE_LINE MATCHES "^#")
                continue()
            endif()
            string(REPLACE "\\" "\\\\" IGNORE_LINE "${IGNORE_LINE}")
            string(REPLACE "." "\\\\." IGNORE_LINE "${IGNORE_LINE}")
            string(REPLACE "*" ".*" IGNORE_LINE "${IGNORE_LINE}")
            string(REPLACE "+" "\\\\+" IGNORE_LINE "${IGNORE_LINE}")
            list(APPEND CPACK_SOURCE_IGNORE_FILES "${IGNORE_LINE}")
        endforeach()
    endif()

    # extra ignored files
    list(APPEND CPACK_SOURCE_IGNORE_FILES
        .editorconfig
        .git
        .gitignore
        .travis.yml
        .appveyor.yml
    )
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_GENERATOR "TGZ;TXZ")
    set(CPACK_SOURCE_GENERATOR "TGZ;TXZ")
    include(CPack)
endif()
