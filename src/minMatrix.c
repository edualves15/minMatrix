#include "minMatrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// unsigned int debug_created_matrices = 0;
// unsigned int debug_destroyed_matrices = 0;

MinMatrix minMatrix_create(unsigned int rows, unsigned int cols) {
  if (rows == 0 || cols == 0) {
    puts("Error creating matrix!: Minimum dimension must be 1x1");
    exit(EXIT_FAILURE);
  }

  unsigned int struct_size = (sizeof(unsigned int) * 2) + sizeof(double **);
  MinMatrix A = (MinMatrix)malloc(struct_size);

  A->rows = rows;
  A->cols = cols;
  A->data = malloc(sizeof(double *) * (rows * cols));

  for (unsigned int i = 0; i < rows; i++)
    A->data[i] = calloc(cols, sizeof(double));

  // debug_created_matrices++;
  return A;
}

MinMatrix minMatrix_from_txt(char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error reading file!");
    exit(EXIT_FAILURE);
  }
  unsigned int i, j, rows, cols;
  fscanf(file, "%u %u", (unsigned int *)&rows, (unsigned int *)&cols);
  MinMatrix A = minMatrix_create(rows, cols);

  for (i = 0; i < A->rows; i++) {
    for (j = 0; j < A->cols; j++) {
      fscanf(file, "%lf", &A->data[i][j]);
    }
  }

  fclose(file);
  return A;
}

MinMatrix minMatrix_from_csv(char *file_path, char type, char delimiter) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error reading file!");
    exit(EXIT_FAILURE);
  }
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int openDoubleQuotes = 0;
  unsigned int openSingleQuotes = 0;
  unsigned int lenToken = 0;
  char *eptr;
  char *token = malloc(CSV_MAX_ROW_WIDHT * sizeof(char));
  char row_buf[CSV_MAX_ROW_WIDHT];
  MinMatrix A = minMatrix_create(1, 1);

  for (row = 0; fgets(row_buf, CSV_MAX_ROW_WIDHT, file); row++) {
    if (row_buf[0] == '\n' || row_buf[0] == '\r') continue;

    // Iterates through all characters in the row
    for (unsigned int i = 0; row_buf[i] != '\n' && row_buf[i] != '\0'; i++) {
      // Check for open quotes
      if (row_buf[i] == '\"') {
        if (openDoubleQuotes == 0)
          openDoubleQuotes = 1;
        else
          openDoubleQuotes = 0;
      } else if (row_buf[i] == '\'') {
        if (openSingleQuotes == 0)
          openSingleQuotes = 1;
        else
          openSingleQuotes = 0;
      }

      // Use character that is not a delimiter or is enclosed in quotation marks
      if (row_buf[i] != delimiter || openDoubleQuotes == 1 ||
          openSingleQuotes == 1) {
        token[lenToken] = row_buf[i];
        lenToken++;
      } else {
        token[lenToken] = '\0';
        if (strlen(token) > 0) {
          if (row == 0)
            minMatrix_add_col(A);  // add all columns at first row
          else if (col == 0)
            minMatrix_add_row(A);  // add new row every new row
          if (type == 'd') A->data[row][col] = strtod(token, &eptr);
        }
        lenToken = 0;
        col++;
      }
    }

    token[lenToken] = '\0';
    if (strlen(token) > 0)
      if (type == 'd') A->data[row][col] = strtod(token, &eptr);
    lenToken = 0;
    col = 0;
  }

  return A;
}

MinMatrix minMatrix_get_row(MinMatrix A, unsigned int row) {
  if (row > A->rows) {
    puts(
        "Error getting row!: the requested row does not exist in the "
        "matrix");
    exit(EXIT_FAILURE);
  }

  MinMatrix B = minMatrix_create(1, A->cols);

  for (unsigned int i = 0; i < A->cols; i++) B->data[0][i] = A->data[row][i];

  return B;
}

MinMatrix minMatrix_get_last_row(MinMatrix A) {
  MinMatrix B = minMatrix_create(1, A->cols);

  for (unsigned int i = 0; i < A->rows; i++)
    B->data[0][i] = A->data[A->rows - 1][i];

  return B;
}

MinMatrix minMatrix_get_col(MinMatrix A, unsigned int col) {
  if (col > A->cols) {
    puts(
        "Error getting column!: the requested column does not exist in the "
        "matrix");
    exit(EXIT_FAILURE);
  }

  MinMatrix B = minMatrix_create(A->rows, 1);

  for (unsigned int i = 0; i < A->rows; i++) B->data[i][0] = A->data[i][col];

  return B;
}

MinMatrix minMatrix_get_last_col(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows, 1);

  for (unsigned int i = 0; i < A->rows; i++)
    B->data[i][0] = A->data[i][A->cols - 1];

  return B;
}

MinMatrix minMatrix_copy(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows, A->cols);

  for (unsigned int i = 0; i < A->rows; i += 1)
    for (unsigned int j = 0; j < A->cols; j += 1) B->data[i][j] = A->data[i][j];

  return B;
}

MinMatrix minMatrix_transpose(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->cols, A->rows);

  for (unsigned int i = 0; i < A->cols; i++)
    for (unsigned int j = 0; j < A->rows; j++) B->data[i][j] = A->data[j][i];

  return B;
}

MinMatrix minMatrix_multiply(MinMatrix A, MinMatrix B) {
  if (A->cols != B->rows) {
    puts(
        "Matrix multiplication error!: The number of columns in matrix 'A' "
        "must equal the number of rows in matrix'B'");
    exit(EXIT_FAILURE);
  }

  MinMatrix C = minMatrix_create(A->rows, B->cols);

  for (unsigned int i = 0; i < A->rows; i++)
    for (unsigned int j = 0; j < B->cols; j++)
      for (unsigned int k = 0; k < B->rows; k++)
        C->data[i][j] += A->data[i][k] * B->data[k][j];

  return C;
}

MinMatrix minMatrix_identity(unsigned int len) {
  if (len <= 0) {
    puts("Error creating identity matrix!: Matrix must have dimension > 0");
    exit(EXIT_FAILURE);
  }

  MinMatrix A = minMatrix_create(len, len);

  for (unsigned int i = 0; i < len; i++)
    for (unsigned int j = 0; j < len; j++)
      if (i == j)
        A->data[i][j] = 1;
      else
        A->data[i][j] = 0;

  return A;
}

MinMatrix minMatrix_minor(MinMatrix A, unsigned int row, unsigned int col) {
  unsigned int i, j, k, l;
  MinMatrix minor = minMatrix_create(A->rows - 1, A->rows - 1);

  for (i = 0, k = 0; i < A->rows; i++)
    if (i != row) {
      for (j = 0, l = 0; j < A->rows; j++)
        if (j != col) {
          minor->data[k][l] = A->data[i][j];
          l++;
        }
      k++;
    }

  // for (i = 0, k = 0; i < A->rows; i++) {
  //   for (j = 0, l = 0; j < A->rows; j++)
  //     if (i != row && j != col) {
  //       minor->data[k][l] = A->data[i][j];
  //       l++;
  //     }
  //   if (i != row) k++;
  // }

  return minor;
}

MinMatrix minMatrix_cofactor(MinMatrix A) {
  MinMatrix minor = minMatrix_create(A->rows - 1, A->rows - 1);
  MinMatrix cof = minMatrix_create(A->rows, A->rows);
  unsigned int row, col, i, j, k, l;

  for (row = 0; row < A->rows; row++) {
    for (col = 0; col < A->rows; col++) {
      // Calculating the minors in line
      // is faster than calling the function.
      for (i = 0, k = 0; i < A->rows; i++)
        if (i != row) {
          for (j = 0, l = 0; j < A->rows; j++)
            if (j != col) {
              minor->data[k][l] = A->data[i][j];
              l++;
            }
          k++;
        }
      if ((row + col) % 2 == 0)
        cof->data[row][col] = minMatrix_determinant(minor);
      else
        cof->data[row][col] = -1 * minMatrix_determinant(minor);

      // if ((row + col) % 2 == 0)
      //   cof->data[row][col] =
      //       minMatrix_determinant(minMatrix_minor(A, row, col));
      // else
      //   cof->data[row][col] =
      //       -1 * minMatrix_determinant(minMatrix_minor(A, row, col));
    }
  }

  minMatrix_destroy(minor);
  return cof;
}

double minMatrix_determinant(MinMatrix A) {
  if (A->cols != A->rows) {
    puts("Error calculating the determinant!: Non square matrix");
    exit(EXIT_FAILURE);
  }

  if (A->rows == 1) return A->data[0][0];

  if (A->rows == 2)
    return A->data[0][0] * A->data[1][1] - A->data[0][1] * A->data[1][0];

  MinMatrix minor = minMatrix_create(A->rows - 1, A->cols - 1);
  unsigned int col, k, l, i, j;
  double det = 0;

  for (col = 0; col < A->rows; col++) {
    for (k = 0, i = 0; i < A->rows; i++)
      if (i != 0) {
        for (l = 0, j = 0; j < A->rows; j++)
          if (j != col) {
            minor->data[k][l] = A->data[i][j];
            l++;
          }
        k++;
      }

    if (col % 2 == 0)
      det += A->data[0][col] * minMatrix_determinant(minor);
    else
      det += (-1 * A->data[0][col]) * minMatrix_determinant(minor);
  }

  minMatrix_destroy(minor);
  return det;
}

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
//       MinMatrix C = minMatrix_minor(A, i, j);
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
  MinMatrix inv = minMatrix_create(A->rows, A->cols);
  double det = minMatrix_determinant(A);

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < A->rows; j++) {
      inv->data[i][j] = adj->data[i][j] / det;
    }
  }

  minMatrix_destroy(cof);
  minMatrix_destroy(adj);
  return inv;
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

void minMatrix_print_properties(MinMatrix A) {
  putchar('\n');
  puts("PROPERTIES OF MATRIX");
  puts("--------------------");
  unsigned int struct_size = (sizeof(unsigned int) * 2) + sizeof(double **);
  unsigned int data_size = sizeof(double *) * (A->rows * A->cols);
  printf("Rows...: %u\nColumns: %u\nBytes..: %u\n", A->rows, A->cols,
         struct_size + data_size);
  putchar('\n');
}

void minMatrix_destroy(MinMatrix A) {
  for (unsigned int i = 0; i < A->rows; i++) {
    free(A->data[i]);
  }
  free(A->data);
  free(A);
  // debug_destroyed_matrices++;
}

void minMatrix_add_row(MinMatrix A) {
  // A = realloc(A, sizeof(MinMatrix) + (sizeof(double *) * (A->rows *
  // A->cols)));
  A->rows += 1;
  A->data = realloc(A->data, sizeof(double *) * (A->rows * A->cols));
  A->data[A->rows - 1] = calloc(A->cols, sizeof(double));
}

void minMatrix_add_col(MinMatrix A) {
  // A = realloc(A, sizeof(MinMatrix) + (sizeof(double *) * (A->rows *
  // A->cols)));
  A->cols += 1;
  A->data = realloc(A->data, sizeof(double *) * (A->rows * A->cols));
  for (unsigned int i = 0; i < A->rows; i++) {
    A->data[i] = realloc(A->data[i], sizeof(double *) * A->cols);
    // A->data[i] = realloc(A->data[i], sizeof(double *) * (A->rows *
    // A->cols));
    A->data[i][A->cols - 1] = 0;
  }
}

/*Function to carry out row operations*/
// void row_operation(MinMatrix multiplier_matrix, MinMatrix matrix, int
// pivot,
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
 This function checks if there is a line containing too many zero's and it
 exits if such a line is found
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
