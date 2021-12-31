#ifndef MINMATRIX_H
#define MINMATRIX_H

#include <stdlib.h>

#define CSV_MAX_ROW_WIDHT 1024

// Declarações de estruturas
typedef struct {
  unsigned int rows;
  unsigned int cols;
  double **data;
} * MinMatrix;

// extern unsigned int debug_create_matrices;
// extern unsigned int debug_destroyed_matrices;

// Protótipos das funções
MinMatrix minMatrix_create(unsigned int rows, unsigned int cols);
MinMatrix minMatrix_from_txt(char *file_path);
MinMatrix minMatrix_from_csv(char *file_path, char type, char delimiter);
MinMatrix minMatrix_copy(MinMatrix A);
MinMatrix minMatrix_get_row(MinMatrix A, unsigned int col);
MinMatrix minMatrix_get_last_row(MinMatrix A);
MinMatrix minMatrix_get_col(MinMatrix A, unsigned int col);
MinMatrix minMatrix_get_last_col(MinMatrix A);
MinMatrix minMatrix_transpose(MinMatrix A);
MinMatrix minMatrix_identity(unsigned int len);
MinMatrix minMatrix_multiply(MinMatrix A, MinMatrix B);
MinMatrix minMatrix_minor(MinMatrix A, unsigned int row, unsigned int col);
MinMatrix minMatrix_cofactor(MinMatrix A);
MinMatrix minMatrix_inverse(MinMatrix A);
double minMatrix_determinant(MinMatrix A);
void minMatrix_print(MinMatrix A, unsigned int decimals, char title[]);
void minMatrix_print_properties(MinMatrix A);
void minMatrix_destroy(MinMatrix A);
void minMatrix_add_row(MinMatrix A);
void minMatrix_add_col(MinMatrix A);

// void row_operation(MinMatrix multiplier_matrix, MinMatrix matrix, int pivot,
//                    unsigned int row_index);
// void row_divide(MinMatrix matrix, int pivot);
// void error_zeros(MinMatrix matrix, unsigned int control_index);
// void minMatrix_row_reduce(MinMatrix matrix, unsigned int zero_control);

#endif