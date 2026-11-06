#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "matrix.h"

static int g_failed = 0;
static int g_total  = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        ++g_total;                                                           \
        if (!(cond)) {                                                       \
            ++g_failed;                                                      \
            std::printf("  [FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #cond); \
        }                                                                    \
    } while (0)

static bool almost_equal(double x, double y, double eps = 1e-9) {
    return std::fabs(x - y) < eps;
}

static void fill_sequence(Matrix& m, double base) {
    double v = base;
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            m.data[i][j] = v;
            v += 1.0;
        }
    }
}

static void test_create_free() {
    Matrix m = create_matrix(3, 4);
    CHECK(matrix_is_valid(m));
    CHECK(m.rows == 3);
    CHECK(m.cols == 4);
    CHECK(m.data[2][3] == 0.0);
    free_matrix(m);
    CHECK(!matrix_is_valid(m));
    CHECK(m.rows == 0 && m.cols == 0);
}

static void test_create_invalid() {
    Matrix m = create_matrix(0, 5);
    CHECK(!matrix_is_valid(m));
    free_matrix(m);
}

static void test_add_sub() {
    Matrix a = create_matrix(2, 2);
    Matrix b = create_matrix(2, 2);
    fill_sequence(a, 1.0);   // 1 2 / 3 4
    fill_sequence(b, 10.0);  // 10 11 / 12 13

    Matrix s{nullptr, 0, 0};
    CHECK(matrix_add(a, b, s) == MAT_OK);
    CHECK(almost_equal(s.data[0][0], 11.0));
    CHECK(almost_equal(s.data[1][1], 17.0));

    Matrix d{nullptr, 0, 0};
    CHECK(matrix_sub(b, a, d) == MAT_OK);
    CHECK(almost_equal(d.data[0][0], 9.0));
    CHECK(almost_equal(d.data[1][1], 9.0));

    free_matrix(a); free_matrix(b); free_matrix(s); free_matrix(d);
}

static void test_dim_mismatch() {
    Matrix a = create_matrix(2, 3);
    Matrix b = create_matrix(2, 3);
    Matrix out{nullptr, 0, 0};

    CHECK(matrix_add(a, b, out) == MAT_OK);
    free_matrix(out);

    Matrix c = create_matrix(3, 3);
    CHECK(matrix_add(a, c, out) == MAT_ERR_DIM);
    CHECK(matrix_mul(a, b, out) == MAT_ERR_DIM);

    free_matrix(a); free_matrix(b); free_matrix(c); free_matrix(out);
}

static void test_null_args() {
    Matrix empty{nullptr, 0, 0};
    Matrix out{nullptr, 0, 0};
    CHECK(matrix_add(empty, empty, out) == MAT_ERR_NULL);
    CHECK(matrix_transpose(empty, out) == MAT_ERR_NULL);
    CHECK(matrix_to_file(empty, "x.txt") == MAT_ERR_NULL);
}

static void test_mul() {
    Matrix a = create_matrix(2, 3);
    Matrix b = create_matrix(3, 2);
    fill_sequence(a, 1.0);  // 1 2 3 / 4 5 6
    fill_sequence(b, 1.0);  // 1 2 / 3 4 / 5 6

    Matrix c{nullptr, 0, 0};
    CHECK(matrix_mul(a, b, c) == MAT_OK);
    CHECK(c.rows == 2 && c.cols == 2);
    CHECK(almost_equal(c.data[0][0], 22.0));
    CHECK(almost_equal(c.data[0][1], 28.0));
    CHECK(almost_equal(c.data[1][0], 49.0));
    CHECK(almost_equal(c.data[1][1], 64.0));

    Matrix e = create_matrix(3, 3);
    for (int i = 0; i < 3; ++i) e.data[i][i] = 1.0;
    Matrix r{nullptr, 0, 0};
    CHECK(matrix_mul(a, e, r) == MAT_OK);
    for (int i = 0; i < a.rows; ++i)
        for (int j = 0; j < a.cols; ++j)
            CHECK(almost_equal(r.data[i][j], a.data[i][j]));

    free_matrix(a); free_matrix(b); free_matrix(c); free_matrix(e); free_matrix(r);
}

static void test_transpose() {
    Matrix a = create_matrix(2, 3);
    fill_sequence(a, 1.0);  // 1 2 3 / 4 5 6
    Matrix t{nullptr, 0, 0};
    CHECK(matrix_transpose(a, t) == MAT_OK);
    CHECK(t.rows == 3 && t.cols == 2);
    CHECK(almost_equal(t.data[0][0], 1.0));
    CHECK(almost_equal(t.data[0][1], 4.0));
    CHECK(almost_equal(t.data[2][0], 3.0));
    free_matrix(a); free_matrix(t);
}

static void test_to_file() {
    const char* path = "test_out_matrix.txt";

    Matrix m = create_matrix(2, 3);
    fill_sequence(m, 1.5);

    CHECK(matrix_to_file(m, path) == MAT_OK);

    std::ifstream fin(path);
    CHECK(fin.is_open());
    std::string line;
    CHECK(static_cast<bool>(std::getline(fin, line)));
    CHECK(line == "2 3");

    double v = 0.0;
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 1.5));
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 2.5));
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 3.5));
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 4.5));
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 5.5));
    CHECK(static_cast<bool>(fin >> v)); CHECK(almost_equal(v, 6.5));
    fin.close();

    CHECK(matrix_to_file(m, "/nonexistent_dir_xyz/a.txt") == MAT_ERR_FILE);

    std::remove(path);
    free_matrix(m);
}

int main() {
    std::printf("Запуск тестов...\n");
    test_create_free();
    test_create_invalid();
    test_add_sub();
    test_dim_mismatch();
    test_null_args();
    test_mul();
    test_transpose();
    test_to_file();

    std::printf("\nВсего проверок: %d, провалено: %d\n", g_total, g_failed);
    if (g_failed == 0) {
        std::printf("ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n");
        return EXIT_SUCCESS;
    }
    std::printf("ЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ\n");
    return EXIT_FAILURE;
}