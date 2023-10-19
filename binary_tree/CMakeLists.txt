enable_testing()

include_directories(.)
add_executable(
    binary_tree_test 
    binary_tree_test.cc
)
target_link_libraries(
    binary_tree_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(binary_tree_test)
