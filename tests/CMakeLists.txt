add_executable(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix PRIVATE matrix_core)

add_test(NAME matrix_unit_tests COMMAND test_matrix)