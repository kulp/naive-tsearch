enable_language(CXX)

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(NO_OUTPUT_DIRS)
endif()

check_include_file(search.h HAS_INCLUDE_SEARCH_H)
if(HAS_INCLUDE_SEARCH_H)
    include(CheckCSourceCompiles)
    check_c_source_compiles("#include <search.h>\nint main(){VISIT v = leaf;}\n" HAS_ENUM_VISIT)
    check_c_source_compiles("#include <search.h>\n#include <stddef.h>\nint main(){tsearch(NULL, NULL, NULL);}\n" HAS_FUNCTION_TSEARCH)
    check_c_source_compiles("#include <search.h>\n#include <stddef.h>\nint main(){tfind(NULL, NULL, NULL);}\n" HAS_FUNCTION_TFIND)
    check_c_source_compiles("#include <search.h>\n#include <stddef.h>\nint main(){tdelete(NULL, NULL, NULL);}\n" HAS_FUNCTION_TDELETE)
    check_c_source_compiles("#include <search.h>\n#include <stddef.h>\nint main(){twalk(NULL, NULL);}\n" HAS_FUNCTION_TWALK)
    check_c_source_compiles("#include <search.h>\n#include <stddef.h>\nint main(){tdestroy(NULL, NULL);}\n" HAS_FUNCTION_TDESTROY)
endif()
if(HAS_INCLUDE_SEARCH_H AND HAS_ENUM_VISIT AND HAS_FUNCTION_TSEARCH AND HAS_FUNCTION_TFIND AND HAS_FUNCTION_TDELETE AND HAS_FUNCTION_TWALK AND HAS_FUNCTION_TDESTROY)
    set(NAIVE_TSEARCH_TEST_COMPARE_SYSTEM_DEFAULT ON)
endif()
option(NAIVE_TSEARCH_TEST_SYSTEM "Compare naive-tsearch with system" "${NAIVE_TSEARCH_TEST_COMPARE_SYSTEM_DEFAULT}")

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

function(naive_tsearch_add_unittest NAME)
    add_executable(${NAME}
        main.cpp
        test_walk.cpp
    )
    target_link_libraries(${NAME} PRIVATE
        Boost::unit_test_framework)
    target_compile_definitions(${NAME} PRIVATE
        $<$<NOT:$<BOOL:${Boost_USE_STATIC_LIBS}>>:BOOST_TEST_DYN_LINK>)
    target_compile_options(${NAME} PRIVATE
        $<IF:$<C_COMPILER_ID:MSVC>,/W4,-Wall -Wextra>)
    set_target_properties(${NAME} PROPERTIES CXX_STANDARD 11)
    add_test(NAME "test_${NAME}" COMMAND ${NAME})
endfunction()

naive_tsearch_add_unittest(test_naive_tsearch)
target_compile_definitions(test_naive_tsearch PRIVATE TEST_NAIVE_TSEARCH)
target_link_libraries(test_naive_tsearch PRIVATE naive-tsearch)

naive_tsearch_add_unittest(test_naive_tsearch_hdronly)
target_compile_definitions(test_naive_tsearch_hdronly PRIVATE TEST_NAIVE_TSEARCH_HDRONLY)
target_compile_options(test_naive_tsearch_hdronly PRIVATE $<IF:$<C_COMPILER_ID:MSVC>,,-Wno-unused-function>)
target_link_libraries(test_naive_tsearch_hdronly PRIVATE naive-tsearch-hdronly)

if(NAIVE_TSEARCH_TEST_SYSTEM)
    naive_tsearch_add_unittest(test_system_tsearch)
    target_compile_definitions(test_system_tsearch PRIVATE TEST_SYSTEM_TSEARCH)
endif()
