enable_testing()

include_directories(.)
add_executable(
    test_bst
    test_bst.cc
)
target_link_libraries(
    test_bst
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_bst)
