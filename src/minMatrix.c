#include "minMatrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MinMatrix minMatrix_create(unsigned int rows, unsigned int cols) {
  // MinMatrix A;
  MinMatrix A = malloc(sizeof(MinMatrix) + (sizeof(double *) * (rows * cols)));

  // alocação da estrutura
  // MinMatrix A = (MinMatrix)malloc(sizeof(MinMatrix));

  A->rows = rows;
  A->cols = cols;
  A->data = malloc(sizeof(double *) * (rows * cols));

  for (unsigned int i = 0; i < rows; i++) {
    A->data[i] = calloc(cols, sizeof(double));
  }

  return A;
}

MinMatrix minMatrix_from_txt(char *file_path) {
  unsigned int i, j, rows, cols;
  FILE *file = fopen(file_path, "r");

  if (file == NULL) {
    perror("Error reading file!\n");
    // exit(EXIT_FAILURE);
  }
  fscanf(file, "%u %u", (unsigned int *)&rows, (unsigned int *)&cols);
  MinMatrix matrix = minMatrix_create(rows, cols);

  for (i = 0; i < matrix->rows; i++) {
    for (j = 0; j < matrix->cols; j++) {
      fscanf(file, "%lf", &matrix->data[i][j]);
    }
  }

  fclose(file);
  return matrix;
}

MinMatrix minMatrix_get_col(MinMatrix A, unsigned int col) {
  MinMatrix B = minMatrix_create(A->rows, 1);

  for (unsigned int i = 0; i < A->rows; i++) {
    B->data[i][0] = A->data[i][col];
  }

  return B;
}

MinMatrix minMatrix_get_last_col(MinMatrix A) {
  puts("------------");
  MinMatrix B = minMatrix_create(A->rows, 1);

  for (unsigned int i = 0; i < A->rows; i++) {
    B->data[i][0] = A->data[i][A->cols - 1];
  }

  return B;
}

MinMatrix minMatrix_copy(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows, A->cols);
  for (unsigned int i = 0; i < A->rows; i += 1) {
    for (unsigned int j = 0; j < A->cols; j += 1) {
      B->data[i][j] = A->data[i][j];
    }
  }
  return B;
}

MinMatrix minMatrix_transpose(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->cols, A->rows);
  for (unsigned int i = 0; i < A->cols; i++) {
    for (unsigned int j = 0; j < A->rows; j++) {
      B->data[i][j] = A->data[j][i];
    }
  }
  return B;
}

MinMatrix minMatrix_multiply(MinMatrix A, MinMatrix B) {
  if (A->cols != B->rows) {
    perror(
        "The number of columns in the first matrix must equal the number of "
        "rows in the second matrix.!\n");
    // exit(EXIT_FAILURE);
  }

  MinMatrix C = minMatrix_create(A->rows, B->cols);

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < B->cols; j++) {
      for (unsigned int k = 0; k < B->rows; k++) {
        C->data[i][j] += A->data[i][k] * B->data[k][j];
      }
    }
  }
  return C;
}

MinMatrix minMatrix_identity(unsigned int len) {
  MinMatrix B = minMatrix_create(len, len);

  for (unsigned int i = 0; i < len; i++) {
    for (unsigned int j = 0; j < len; j++) {
      if (i == j) {
        B->data[i][j] = 1;
      } else {
        B->data[i][j] = 0;
      }
    }
  }
  return B;
}

MinMatrix minMatrix_get_minor(MinMatrix A, unsigned int row, unsigned int col) {
  unsigned int k = 0, l = 0;
  MinMatrix B = minMatrix_create(A->rows - 1, A->rows - 1);

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < A->rows; j++) {
      if (i != row && j != col) {
        B->data[k][l] = A->data[i][j];
        l++;
      }
    }
    if (i != row) k++;
    l = 0;
  }

  return B;
}

MinMatrix minMatrix_cofactor(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows - 1, A->rows - 1);
  MinMatrix C = minMatrix_create(A->rows, A->rows);
  unsigned int i, j, ii, jj;

  for (unsigned int row = 0; row < A->rows; row++) {
    for (unsigned int col = 0; col < A->rows; col++) {
      for (ii = 0, i = 0; i < A->rows;) {
        if (i != row) {
          for (jj = 0, j = 0; j < A->rows;) {
            if (j != col) {
              B->data[ii][jj] = A->data[i][j];
              j++;
              jj++;
            } else {
              j++;
            }
          }
          i++;
          ii++;
        } else {
          i++;
        }
      }
      if (((row % 2 == 0) && (col % 2 == 0)) ||
          ((row % 2 != 0) && (col % 2 != 0)))
        C->data[row][col] = minMatrix_determinant(B);
      else {
        C->data[row][col] = -1 * minMatrix_determinant(B);
      }
    }
  }

  return C;
}

double minMatrix_determinant(MinMatrix A) {
  if (A->cols != A->rows) {
    perror("Non square matrix!\n");
    // exit(EXIT_FAILURE);
  }

  double det = 0;

  if (A->rows == 1) {
    return A->data[0][0];
  } else if (A->rows == 2) {
    return (A->data[0][0] * A->data[1][1] - A->data[0][1] * A->data[1][0]);
  } else {
    for (unsigned int i = 0; i < A->rows; i++) {
      MinMatrix B = minMatrix_get_minor(A, 0, i);
      det += A->data[0][i] * pow(-1, i) * minMatrix_determinant(B);
      minMatrix_destroy(B);
    }
    return det;
  }
}

// double minMatrix_determinant(MinMatrix A) {
//   MinMatrix B = minMatrix_create(A->rows - 1, A->cols - 1);
//   double det = 0;

//   if (A->rows == 1) {
//     return A->data[0][0];
//   } else if (A->rows == 2) {
//     return A->data[0][0] * A->data[1][1] - A->data[0][1] * A->data[1][0];
//   } else {
//     for (unsigned int i = 0; i < A->rows; i++) {
//       for (unsigned int ii = 0, j = 0; j < A->rows;) {
//         if (j != 0) {
//           for (unsigned int jj = 0, k = 0; k < A->rows;) {
//             if (k != i) {
//               B->data[ii][jj] = A->data[j][k];
//               k++;
//               jj++;
//             } else {
//               k++;
//             }
//           }
//           j++;
//           ii++;
//         } else {
//           j++;
//         }
//       }
//       if (i % 2 == 0) {
//         det += A->data[0][i] * minMatrix_determinant(B);
//       } else {
//         det += (-1 * A->data[0][i]) * minMatrix_determinant(B);
//       }
//     }

//     minMatrix_destroy(B);
//     return det;
//   }
// }

// MinMatrix minMatrix_inverse(MinMatrix A) {
//   MinMatrix B = minMatrix_create(A->cols, A->cols);
//   double det = minMatrix_determinant(A);

//   if (A->cols == 2) {
//     B->data[0][0] = A->data[1][1] / det;
//     B->data[0][1] = (-1) * A->data[0][1] / det;
//     B->data[1][0] = (-1) * A->data[1][0] / det;
//     B->data[1][1] = A->data[0][0] / det;
//     return B;
//   }

//   for (unsigned int i = 0; i < A->cols; i++) {
//     for (unsigned int j = 0; j < A->cols; j++) {
//       MinMatrix C = minMatrix_get_minor(A, i, j);
//       B->data[i][j] = (pow(-1, i + j) * minMatrix_determinant(C)) / det;
//       minMatrix_destroy(C);
//     }
//   }

//   MinMatrix D = minMatrix_transpose(B);
//   minMatrix_destroy(B);
//   return D;
// }

// INVERSE OF MATRIX USING THE ADJUGATE METHOD
// MATRIZ INVERSA COM O MÉTODO DA INVERSÃO POR MATRIZ ADJUNTA
MinMatrix minMatrix_inverse(MinMatrix A) {
  if (A->cols != A->rows) {
    perror("Non square matrix!\n");
    // exit(EXIT_FAILURE);
  }

  MinMatrix cof = minMatrix_cofactor(A);     // cofactor matrix
  MinMatrix adj = minMatrix_transpose(cof);  // adjoint matrix
  MinMatrix B = minMatrix_create(A->rows, A->cols);
  double det = minMatrix_determinant(A);

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < A->rows; j++) {
      B->data[i][j] = adj->data[i][j] / det;
    }
  }

  minMatrix_destroy(cof);
  minMatrix_destroy(adj);
  return B;
}

void minMatrix_print(MinMatrix A, unsigned int decimals, char title[]) {
  unsigned int len_title = strlen(title);

  putchar('\n');
  puts(title);
  for (unsigned int i = 0; i < len_title; i++) {
    putchar('-');
  }
  putchar('\n');

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < A->cols; j++) {
      printf("%-8.*lf  ", decimals, A->data[i][j]);
    }
    putchar('\n');
  }
  putchar('\n');
}

void minMatrix_destroy(MinMatrix A) {
  for (unsigned int i = 0; i < A->rows; i++) {
    free(A->data[i]);
  }
  free(A->data);
  free(A);
}

void minMatrix_add_row(MinMatrix A) {
  A->rows += 1;
  A->data = realloc(A->data, sizeof(double *) * (A->rows * A->cols));
  A->data[A->rows - 1] = calloc(A->cols, sizeof(double));
}

void minMatrix_add_col(MinMatrix A) {
  A->cols += 1;
  A->data = realloc(A->data, sizeof(double *) * (A->rows * A->cols));
  for (unsigned int i = 0; i < A->rows; i++) {
    A->data[i] = realloc(A->data[i], sizeof(double *) * (A->rows * A->cols));
    A->data[i][A->cols - 1] = 0;
  }
}

/*Function to carry out row operations*/
// void row_operation(MinMatrix multiplier_matrix, MinMatrix matrix, int pivot,
//                    unsigned int row_index) {
//   double multiplier =
//       (matrix->data[row_index][pivot] / matrix->data[pivot][pivot]);
//   // Loop which checks if matrix is provided to store the multiplier
//   if (multiplier_matrix != NULL) {
//     multiplier_matrix->data[row_index][pivot] = multiplier;
//   }

//   for (unsigned int j = 0; j < matrix->cols; j++) {
//     matrix->data[row_index][j] -= multiplier * matrix->data[pivot][j];
//   }
// }

// /*Function which divides all row entries by the value of a the diagonal */
// void row_divide(MinMatrix matrix, int pivot) {
//   double divisor = matrix->data[pivot][pivot], result;

//   for (unsigned int j = pivot; j < matrix->cols; j++) {
//     result = (matrix->data[pivot][j] / divisor);
//     matrix->data[pivot][j] = result;
//   }
// }

/*
 This function checks if there is a line containing too many zero's and it exits
 if such a line is found
*/
// void error_zeros(MinMatrix matrix, unsigned int control_index) {
//   unsigned int count;

//   for (unsigned int i = 0; i < matrix->rows; i++) {
//     count = 0;
//     for (unsigned int j = 0; j < matrix->cols; j++) {
//       if (matrix->data[i][j] == 0) {
//         count++;
//       } else {
//         return;
//       }
//       if (count == control_index) {
//         // "ERROR: "\nRow %d contains %d zeros\n", i, control_index
//       }
//     }
//   }
// }

// void minMatrix_row_reduce(MinMatrix A, unsigned int zero_control) {
//   int pivot, row_index;
//   for (pivot = 0; pivot < A->rows; pivot++) {
//     error_zeros(A, zero_control);  // Function checks if there are too many
//                                    // zeros in a single row
//     if ((A->data[pivot][pivot] != 1) && (A->data[pivot][pivot] != 0)) {
//       row_divide(A, pivot);
//     }

//     for (row_index = pivot + 1; row_index < A->rows; row_index++) {
//       if (A->data[pivot][pivot] != 0) {
//         row_operation(NULL, A, pivot, row_index);
//       }
//     }

//     for (row_index = pivot - 1; row_index >= 0; row_index--) {
//       if (A->data[pivot][pivot] != 0) {
//         row_operation(NULL, A, pivot, row_index);
//       }
//     }
//   }
// }
