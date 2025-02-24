# cmake-format: off
# src/beman/optional/tests/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include(GoogleTest)

# Tests
# add_executable(beman_optional_test)

target_sources(
    beman_optional_test
    PRIVATE
        optional.t.cpp
        optional_constexpr.t.cpp
        optional_monadic.t.cpp
        optional_range_support.t.cpp
        optional_ref.t.cpp
        optional_ref_monadic.t.cpp
)

target_sources(
    beman_optional_test
    PRIVATE
        FILE_SET beman_optional_test_headers
        TYPE HEADERS
        FILES test_types.hpp test_utilities.hpp
)

target_link_libraries(
    beman_optional_test
    PRIVATE beman_optional GTest::gtest GTest::gtest_main
)

# Issue #32: Re-enable ASAN run CI/clang-19.
#
# Note: clang-19 + gtest_discover_tests + Asan setup causes errors on some
# platforms. Temporary switch to gtest_add_tests and skip some Asan checks.
# Change also applied for CI flows.
gtest_add_tests(TARGET beman_optional_test "" AUTO)

add_library(constructor_fails test_constructor_fail.cpp)

target_link_libraries(constructor_fails PRIVATE beman_optional)

set_target_properties(
    constructor_fails
    PROPERTIES EXCLUDE_FROM_ALL true EXCLUDE_FROM_DEFAULT_BUILD true
)

add_test(
    NAME constructor_fails
    COMMAND
        ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}" --target
        constructor_fails --config $<CONFIGURATION>
)

set_tests_properties(constructor_fails PROPERTIES WILL_FAIL true)
# Alternatively -- check for a particular regex to pass
# set_tests_properties(constructor_fails PROPERTIES PASS_REGULAR_EXPRESSION
# "Assert true")
