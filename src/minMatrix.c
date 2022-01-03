/*
File name
Author
Origin date
Module version number
Compiler version used to compile the code
The intended target
Copyright information
Miscellaneous notes
Revision information
*/

#include "minMatrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// unsigned int debug_created_matrices = 0;
// unsigned int debug_destroyed_matrices = 0;

/*
 * Function: minMatrix_create
 * --------------------------
 * Aloca a memória necessária para a estrutura da matriz e preenche
 * todos os campos com zeros.
 *
 * rows: número de linhas da matriz
 * cols: número de colunas da matriz
 *
 * retorno: a matriz criada
 */
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

/*
 * Create a matrix from tabular numeric data in a text file.
 *
 * @param {file_path} text file path
 *
 * @returns Matrix containing text file data
 */
MinMatrix minMatrix_from_txt(char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error reading text file!");
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

/*
 * Creates a matrix and imports data from a .csv file if this data is numeric.
 * Numbers in quotes are also accepted.
 *
 * @param {file_path} endereço do arquivo
 * @param {delimiter} caracter que separa os elementos
 * @param {has_header} indica se há um header na primeira linha do arquivo .csv
 *
 * @returns Matrix containing .csv file data
 */
MinMatrix minMatrix_from_csv(char *file_path, char delimiter,
                             unsigned int has_header) {
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

  while (fgets(row_buf, CSV_MAX_ROW_WIDHT, file)) {
    if (row == 0 && has_header) {
      has_header = 0;
      continue;
    }

    if (row_buf[0] == '\n' || row_buf[0] == '\r') {
      continue;
    }

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
          A->data[row][col] = strtod(token, &eptr);
        }
        lenToken = 0;
        col++;
      }
    }

    token[lenToken] = '\0';
    if (strlen(token) > 0) A->data[row][col] = strtod(token, &eptr);
    lenToken = 0;
    col = 0;
    row++;
  }

  free(token);
  return A;
}

/*
 * Function: minMatrix_get_row
 * ---------------------------
 * Copia uma linha específica de uma matriz
 *
 * A: matriz de entrada
 * row: a linha especificada
 *
 * retorno: matriz contendo apenas a linha especificada
 */
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

/*
 * Function: minMatrix_get_last_row
 * --------------------------------
 * Copia a última linha de uma matriz
 *
 * A: matriz de entrada
 *
 * retorno: matriz contendo a última linha da matriz de entrada
 */
MinMatrix minMatrix_get_last_row(MinMatrix A) {
  MinMatrix B = minMatrix_create(1, A->cols);

  for (unsigned int i = 0; i < A->rows; i++)
    B->data[0][i] = A->data[A->rows - 1][i];

  return B;
}

/*
 * Function: minMatrix_get_col
 * ---------------------------
 * Copia uma coluna específica de uma matriz
 *
 * A: matriz de entrada
 * row: a coluna especificada
 *
 * retorno: matriz contendo apenas a coluna especificada
 */
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

/*
 * Function: minMatrix_get_last_col
 * --------------------------------
 * Copia a última coluna de uma matriz
 *
 * A: matriz de entrada
 *
 * retorno: matriz contendo a última coluna da matriz de entrada
 */
MinMatrix minMatrix_get_last_col(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows, 1);

  for (unsigned int i = 0; i < A->rows; i++)
    B->data[i][0] = A->data[i][A->cols - 1];

  return B;
}

/*
 * Function: minMatrix_copy
 * ------------------------
 * Copia uma matriz para nova matriz
 *
 * A: matriz de entrada
 *
 * retorno: cópia da matriz de entrada
 */
MinMatrix minMatrix_copy(MinMatrix A) {
  MinMatrix B = minMatrix_create(A->rows, A->cols);

  for (unsigned int i = 0; i < A->rows; i += 1)
    for (unsigned int j = 0; j < A->cols; j += 1) B->data[i][j] = A->data[i][j];

  return B;
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
    A->data[i] = realloc(A->data[i], sizeof(double *) * A->cols);
    A->data[i][A->cols - 1] = 0;
  }
}

/*
 * Function: minMatrix_sum
 * -----------------------
 * Soma duas matrizes com dimensões iguais
 *
 * A: matriz 1
 * B: matriz 2
 *
 * retorno: matriz com a soma das duas matrizes de entrada
 */
MinMatrix minMatrix_sum(MinMatrix A, MinMatrix B) {
  if (A->rows != B->rows || A->cols != B->cols) {
    puts(
        "Error adding matrices!: Matrices must have equal number of rows and "
        "columns");
    exit(EXIT_FAILURE);
  }

  MinMatrix sum = minMatrix_create(A->rows, A->cols);
  for (unsigned int i = 0; i < A->rows; i++)
    for (unsigned int j = 0; j < A->cols; j++)
      sum->data[i][j] = A->data[i][j] + B->data[i][j];

  return sum;
}

/*
 * Function: minMatrix_diff
 * ------------------------
 * Diferença entre duas matrizes
 *
 * A: matriz 1
 * B: matriz 2
 *
 * retorno: matriz com a diferença entre as duas matrizes de entrada
 */
MinMatrix minMatrix_diff(MinMatrix A, MinMatrix B) {
  if (A->rows != B->rows || A->cols != B->cols) {
    puts(
        "error subtracting matrices!: Matrices must have equal number of rows "
        "and columns");
    exit(EXIT_FAILURE);
  }

  MinMatrix diff = minMatrix_create(A->rows, A->cols);
  for (unsigned int i = 0; i < A->rows; i++)
    for (unsigned int j = 0; j < A->cols; j++)
      diff->data[i][j] = A->data[i][j] - B->data[i][j];

  return diff;
}

/*
 * Function: minMatrix_multiply
 * ----------------------------
 * Multiplica duas matrizes quando o número de colunas da primeira matriz é
 * igual ao número de linhas da segunda matriz. No caso da multiplicação de
 * matrizes a ordem dos fatores altera o produto, portanto, A*B ≠ B*A. Este
 * algoritmo usa o método intuitivo (padrão) de multiplicação de matrizes
 *
 * A: matriz 1
 * B: matriz 2
 *
 * retorno: matriz com o produto das duas matrizes de entrada
 */
MinMatrix minMatrix_multiply(MinMatrix A, MinMatrix B) {
  if (A->cols != B->rows) {
    puts(
        "Matrix multiplication error!: The number of columns in matrix 'A' "
        "must equal the number of rows in matrix'B'");
    exit(EXIT_FAILURE);
  }

  MinMatrix prod = minMatrix_create(A->rows, B->cols);

  for (unsigned int i = 0; i < A->rows; i++)
    for (unsigned int j = 0; j < B->cols; j++)
      for (unsigned int k = 0; k < B->rows; k++)
        prod->data[i][j] += A->data[i][k] * B->data[k][j];

  return prod;
}

/*
 * Function: minMatrix_multiply_by_number
 * --------------------------------------
 * Multiplica uma matriz por um número. Cada elemento da matriz é multiplicado
 * pelo número especificado
 *
 * A: matriz de entrada
 * number: multiplicador
 *
 * retorno: matriz com o produto da matrize de entrada e o número
 */
MinMatrix minMatrix_multiply_by_number(MinMatrix A, double number) {
  MinMatrix prod = minMatrix_create(A->rows, A->cols);

  for (unsigned int i = 0; i < A->rows; i++)
    for (unsigned int j = 0; j < A->cols; j++)
      prod->data[i][j] = A->data[i][j] * number;

  return prod;
}

/*
 * Function: minMatrix_transpose
 * -----------------------------
 * Cria nova matriz trocando linhas por colunas da matriz de entrada.
 *
 * A: matriz de entrada
 *
 * retorno: matriz transposta
 */
MinMatrix minMatrix_transpose(MinMatrix A) {
  MinMatrix T = minMatrix_create(A->cols, A->rows);

  for (unsigned int i = 0; i < A->cols; i++)
    for (unsigned int j = 0; j < A->rows; j++) T->data[i][j] = A->data[j][i];

  return T;
}

/*
 * Function: minMatrix_identity
 * ----------------------------
 * Cria uma matriz diagonal, cujos elementos da diagonal principal
 * são todos iguais a 1.
 *
 * len: dimensão da matriz a ser criada (ordem da matriz)
 *
 * retorno: matriz identidade
 */
MinMatrix minMatrix_identity(unsigned int len) {
  if (len <= 0) {
    puts("Error creating identity matrix!: Matrix must have dimension > 0");
    exit(EXIT_FAILURE);
  }

  MinMatrix I = minMatrix_create(len, len);

  for (unsigned int i = 0; i < len; i++)
    for (unsigned int j = 0; j < len; j++)
      if (i == j) I->data[i][j] = 1;
  // else
  //   I->data[i][j] = 0;

  return I;
}

/*
 * Function: minMatrix_minor
 * -------------------------
 * Cria uma matriz de menores obtida pela remoção
 * de uma linha e uma coluna da matriz de entrada.
 *
 * row: linha a ser removida
 * col: coluna a ser removida
 *
 * retorno: matriz de menores
 */
MinMatrix minMatrix_minor(MinMatrix A, unsigned int row, unsigned int col) {
  if (A->cols != A->rows) {
    puts("Error calculating minors!: Non square matrix");
    exit(EXIT_FAILURE);
  }
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

  return minor;
}

/*
 * Function: minMatrix_cofactor
 * ----------------------------
 * Cria uma matriz de cofatores a partir de uma matriz de entrada.
 * O cofator de cada elemento é o determinante da matriz de menores,
 * que por sua vez é obtida a partir da matriz de entrada. O sinal do
 * cofator deve ser invertido nas posições ímpares da matriz (-1)
 *
 * A: Matriz de entrada
 *
 * retorno: matriz de cofatores
 */
MinMatrix minMatrix_cofactor(MinMatrix A) {
  if (A->cols != A->rows) {
    puts("Error calculating cofactor!: Non square matrix");
    exit(EXIT_FAILURE);
  }

  MinMatrix minor = minMatrix_create(A->rows - 1, A->rows - 1);
  MinMatrix cof = minMatrix_create(A->rows, A->rows);
  unsigned int row, col, i, j, k, l;

  for (row = 0; row < A->rows; row++) {
    for (col = 0; col < A->rows; col++) {
      // Calculate minors inline
      // This is 30% faster than calling the function
      for (i = 0, k = 0; i < A->rows; i++)
        if (i != row) {
          for (j = 0, l = 0; j < A->rows; j++)
            if (j != col) {
              minor->data[k][l] = A->data[i][j];
              l++;
            }
          k++;
        }
      // Inverts the sign if the sum of the positions in each dimension is odd
      if ((row + col) % 2 == 0)
        cof->data[row][col] = minMatrix_determinant(minor);
      else
        cof->data[row][col] = -1 * minMatrix_determinant(minor);

      // Calculate minors with function call every iteration
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

/*
 * Function: minMatrix_determinant
 * -------------------------------
 * Calcula o determinante fazendo uso do Teorema de Laplace
 *
 * A: Matriz de entrada
 *
 * retorno: determinante (double)
 */
double minMatrix_determinant(MinMatrix A) {
  if (A->cols != A->rows) {
    puts("Error calculating determinant!: Non square matrix");
    exit(EXIT_FAILURE);
  }

  if (A->rows == 1) return A->data[0][0];

  if (A->rows == 2)
    return A->data[0][0] * A->data[1][1] - A->data[0][1] * A->data[1][0];

  MinMatrix minor = minMatrix_create(A->rows - 1, A->cols - 1);
  unsigned int col, i, j, k, l;
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

/*
 * Function: minMatrix_inverse
 * ---------------------------
 * Cria uma matriz inversa da matriz de entrada.
 * Esta função usa o método da inversão por matriz adjunta.
 *
 * A: Matriz de entrada
 *
 * retorno: matriz inversa
 */
MinMatrix minMatrix_inverse(MinMatrix A) {
  if (A->cols != A->rows) {
    puts("Error calculating matrix inverse!: Non square matrix");
    exit(EXIT_FAILURE);
  }

  double det = minMatrix_determinant(A);
  if (det == 0) {
    puts("Array inversion error!: Determinant=0");
    exit(EXIT_FAILURE);
  }

  MinMatrix cof = minMatrix_cofactor(A);     // cofactor matrix
  MinMatrix adj = minMatrix_transpose(cof);  // adjoint matrix
  MinMatrix inv = minMatrix_create(A->rows, A->cols);

  for (unsigned int i = 0; i < A->rows; i++) {
    for (unsigned int j = 0; j < A->rows; j++) {
      inv->data[i][j] = adj->data[i][j] / det;
    }
  }

  minMatrix_destroy(cof);
  minMatrix_destroy(adj);
  return inv;
}

/*
 * Function: minMatrix_print
 * -------------------------
 * Imprime uma matriz na saída padrão.
 *
 * A: Matriz de entrada
 * decimals: Número de casas decimais dos elementos a serem impressos
 * title: Título que será impresso com a matriz
 */
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

/*
 * Function: minMatrix_print_properties
 * ------------------------------------
 * Imprime os atributos de uma matriz:
 * - Número de linhas;
 * - Número de colunas;
 * - Tamanho em bytes.
 *
 * A: Matriz de entrada
 */
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

/*
 * Function: minMatrix_destroy
 * ---------------------------
 * Desaloca os endereços usados pela matriz (arrays e estruturas):
 */
void minMatrix_destroy(MinMatrix A) {
  for (unsigned int i = 0; i < A->rows; i++) {
    free(A->data[i]);
  }
  free(A->data);
  free(A);
  // debug_destroyed_matrices++;
}
