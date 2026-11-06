#ifndef MATRIX_CALCULATOR_MATRIX_H
#define MATRIX_CALCULATOR_MATRIX_H

enum MatrixStatus {
    MAT_OK        =  0,
    MAT_ERR_NULL  = -1,
    MAT_ERR_ALLOC = -2, 
    MAT_ERR_SIZE  = -3,
    MAT_ERR_DIM   = -4,
    MAT_ERR_FILE  = -5 
};

struct Matrix {
    double** data; 
    int rows;                        
    int cols;     
};

Matrix create_matrix(int rows, int cols);

void free_matrix(Matrix& m);

bool matrix_is_valid(const Matrix& m);

int matrix_copy(const Matrix& src, Matrix& dst);

int matrix_add(const Matrix& a, const Matrix& b, Matrix& out);

int matrix_sub(const Matrix& a, const Matrix& b, Matrix& out);

int matrix_mul(const Matrix& a, const Matrix& b, Matrix& out);

int matrix_to_file(const Matrix& m, const char* filename);

int matrix_transpose(const Matrix& a, Matrix& out);

int matrix_to_file(const Matrix& m, const char* filename);

void print_matrix(const Matrix& m);
const char* matrix_status_str(int status);

#endif 