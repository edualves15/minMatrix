#include <stdio.h>

#include "minMatrix.h"

int main() {
  MinMatrix A = minMatrix_from_txt("data\\dataX.txt");
  minMatrix_print(A, "MATRIX 'A' FROM .TXT FILE");
  printf("Rows...: %u\nColumns: %u\n", A.rows, A.cols);

  MinMatrix B = minMatrix_from_txt("data\\dataY.txt");
  minMatrix_print(B, "MATRIX 'B' FROM .TXT FILE");

  MinMatrix C = minMatrix_copy(B);
  minMatrix_print(C, "MATRIX 'C' IS A COPY OF MATRIX 'B'");

  MinMatrix C1 = minMatrix_get_col(C, 0);
  minMatrix_print(C1, "COLUMN 0 FROM MATRIX 'C'");

  MinMatrix C2 = minMatrix_get_last_col(C);
  minMatrix_print(C2, "LAST COLUMN FROM MATRIX 'C'");

  MinMatrix D = minMatrix_multiply(A, B);
  minMatrix_print(D, "MATRIX 'A' * MATRIX 'C'");

  MinMatrix E = minMatrix_identity(C);
  minMatrix_print(E, "MATRIX IDENTITY");

  MinMatrix F = minMatrix_transpose(C);
  minMatrix_print(F, "MATRIX 'F' IS MATRIX 'C' TRANSPOSED");

  MinMatrix G = minMatrix_get_minor(A, 0, 0);
  minMatrix_print(G, "MINOR MATRIX FROM MATRIX 'A' AT ROW 0 AND COLUMN 0");

  double det = minMatrix_determinant(A);
  puts("\nDETERMINANT OF MATRIX 'A'");
  puts("-------------------------");
  printf("%lf\n", det);

  MinMatrix H = minMatrix_inverse(A);
  minMatrix_print(H, "INVERSE OF MATRIX 'A'");

  MinMatrix I = minMatrix_cofactor(A);
  minMatrix_print(I, "COFACTOR OF MATRIX 'A'");

  minMatrix_destroy(A);
  minMatrix_destroy(B);
  minMatrix_destroy(C);
  minMatrix_destroy(C1);
  minMatrix_destroy(C2);
  minMatrix_destroy(D);
  minMatrix_destroy(E);
  minMatrix_destroy(F);
  minMatrix_destroy(G);
  minMatrix_destroy(I);

  return 0;
}
