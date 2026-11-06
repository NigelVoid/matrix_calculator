#include "matrix.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Matrix create_matrix(int rows, int cols) {
    Matrix m{nullptr, 0, 0};
    if (rows <= 0 || cols <= 0) {
        return m;
    }

    m.data = new (std::nothrow) double*[static_cast<std::size_t>(rows)];
    if (m.data == nullptr) {
        return m;
    }

    for (int i = 0; i < rows; ++i) {
        m.data[i] = new (std::nothrow) double[static_cast<std::size_t>(cols)]();
        if (m.data[i] == nullptr) {
            for (int j = 0; j < i; ++j) {
                delete[] m.data[j];
            }
            delete[] m.data;
            return Matrix{nullptr, 0, 0};
        }
    }

    m.rows = rows;
    m.cols = cols;
    return m;
}

void free_matrix(Matrix& m) {
    if (m.data != nullptr) {
        for (int i = 0; i < m.rows; ++i) {
            delete[] m.data[i];
        }
        delete[] m.data;
    }
    m.data = nullptr;
    m.rows = 0;
    m.cols = 0;
}

bool matrix_is_valid(const Matrix& m) {
    return m.data != nullptr && m.rows > 0 && m.cols > 0;
}

int matrix_copy(const Matrix& src, Matrix& dst) {
    if (!matrix_is_valid(src)) {
        return MAT_ERR_NULL;
    }
    free_matrix(dst);
    dst = create_matrix(src.rows, src.cols);
    if (!matrix_is_valid(dst)) {
        return MAT_ERR_ALLOC;
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.data[i][j] = src.data[i][j];
        }
    }
    return MAT_OK;
}

int matrix_add(const Matrix& a, const Matrix& b, Matrix& out) {
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        return MAT_ERR_NULL;
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        return MAT_ERR_DIM;
    }

    free_matrix(out);
    out = create_matrix(a.rows, a.cols);
    if (!matrix_is_valid(out)) {
        return MAT_ERR_ALLOC;
    }

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            out.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return MAT_OK;
}

int matrix_sub(const Matrix& a, const Matrix& b, Matrix& out) {
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        return MAT_ERR_NULL;
    }
    if (a.rows != b.rows || a.cols != b.cols) {
        return MAT_ERR_DIM;
    }

    free_matrix(out);
    out = create_matrix(a.rows, a.cols);
    if (!matrix_is_valid(out)) {
        return MAT_ERR_ALLOC;
    }

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            out.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return MAT_OK;
}

int matrix_mul(const Matrix& a, const Matrix& b, Matrix& out) {
    if (!matrix_is_valid(a) || !matrix_is_valid(b)) {
        return MAT_ERR_NULL;
    }
    if (a.cols != b.rows) {
        return MAT_ERR_DIM;
    }

    free_matrix(out);
    out = create_matrix(a.rows, b.cols);
    if (!matrix_is_valid(out)) {
        return MAT_ERR_ALLOC;
    }

    for (int i = 0; i < a.rows; ++i) {
        double* row_out = out.data[i];
        for (int k = 0; k < a.cols; ++k) {
            const double aik = a.data[i][k];
            if (aik == 0.0) {
                continue; 
            }
            const double* row_b = b.data[k];
            for (int j = 0; j < b.cols; ++j) {
                row_out[j] += aik * row_b[j];
            }
        }
    }
    return MAT_OK;
}

int matrix_transpose(const Matrix& a, Matrix& out) {
    if (!matrix_is_valid(a)) {
        return MAT_ERR_NULL;
    }

    free_matrix(out);
    out = create_matrix(a.cols, a.rows);
    if (!matrix_is_valid(out)) {
        return MAT_ERR_ALLOC;
    }

    for (int i = 0; i < a.rows; ++i) {
        for (int j = 0; j < a.cols; ++j) {
            out.data[j][i] = a.data[i][j];
        }
    }
    return MAT_OK;
}

int matrix_to_file(const Matrix& m, const char* filename) {
    if (!matrix_is_valid(m) || filename == nullptr) {
        return MAT_ERR_NULL;
    }

    std::ofstream fout(filename);
    if (!fout.is_open()) {
        return MAT_ERR_FILE;
    }

    fout << m.rows << ' ' << m.cols << '\n';
    fout << std::setprecision(10);
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            if (j != 0) {
                fout << ' ';
            }
            fout << m.data[i][j];
        }
        fout << '\n';
    }
    fout.flush();

    if (!fout.good()) {
        return MAT_ERR_FILE;
    }
    return MAT_OK;
}

void print_matrix(const Matrix& m) {
    if (!matrix_is_valid(m)) {
        std::cout << "(пустая матрица)\n";
        return;
    }

    std::size_t width = 1;
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            std::ostringstream oss;
            oss << std::setprecision(6) << m.data[i][j];
            width = std::max(width, oss.str().size());
        }
    }

    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            std::cout << std::setw(static_cast<int>(width)) << std::setprecision(6)
                      << m.data[i][j] << (j + 1 == m.cols ? '\n' : ' ');
        }
    }
}

const char* matrix_status_str(int status) {
    switch (status) {
        case MAT_OK:        return "OK";
        case MAT_ERR_NULL:  return "передан пустой указатель / матрица не создана";
        case MAT_ERR_ALLOC: return "не удалось выделить память";
        case MAT_ERR_SIZE:  return "недопустимые размеры матрицы";
        case MAT_ERR_DIM:   return "несовместимые размерности операндов";
        case MAT_ERR_FILE:  return "не удалось открыть/записать файл";
        default:            return "неизвестный код ошибки";
    }
}