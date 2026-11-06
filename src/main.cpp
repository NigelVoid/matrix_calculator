#include <cstdlib>
#include <iostream>

#include "matrix.h"

int main() {
    Matrix a = create_matrix(2, 3);
    Matrix b = create_matrix(3, 2);

    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        std::cerr << "Ошибка: не удалось создать матрицы\n";
        return EXIT_FAILURE;
    }

    // a = [[1 2 3], [4 5 6]]
    int value = 1;
    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            a.data[i][j] = static_cast<double>(value++);
        }
    }

    // b = [[1 3], [2 4], [3 5]]
    for (int i = 0; i < b.rows; ++i) {
        for (int j = 0; j < b.cols; ++j) {
            b.data[i][j] = static_cast<double>((i + 1) + 2 * j);
        }
    }

    std::cout << "A =\n";
    print_matrix(a);
    std::cout << "B =\n";
    print_matrix(b);

    Matrix sum{nullptr, 0, 0};
    Matrix prod{nullptr, 0, 0};
    Matrix trans{nullptr, 0, 0};

    if (matrix_mul(a, b, prod) != MAT_OK) {
        std::cerr << "Ошибка умножения\n";
        return EXIT_FAILURE;
    }
    std::cout << "A * B =\n";
    print_matrix(prod);

    matrix_transpose(prod, trans);
    std::cout << "(A * B)^T =\n";
    print_matrix(trans);

    const int rc = matrix_to_file(prod, "out_matrix.txt");
    if (rc != MAT_OK) {
        std::cerr << "matrix_to_file: " << matrix_status_str(rc) << '\n';
    } else {
        std::cout << "\nРезультат сохранён в out_matrix.txt\n";
    }

    free_matrix(a);
    free_matrix(b);
    free_matrix(sum);
    free_matrix(prod);
    free_matrix(trans);
    return EXIT_SUCCESS;
}