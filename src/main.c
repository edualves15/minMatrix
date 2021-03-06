#include <stdio.h>
#include <time.h>

#include "minMatrix.h"

int main() {
  MinMatrix A = minMatrix_from_txt("data\\dataX.txt");
  minMatrix_print(A, 0, "MATRIX 'A' FROM .TXT FILE");

  MinMatrix B = minMatrix_from_txt("data\\dataY.txt");
  minMatrix_print(B, 0, "MATRIX 'B' FROM .TXT FILE");

  MinMatrix B2 = minMatrix_from_csv("data\\dataX.csv", ',', 0);
  minMatrix_print(B2, 0, "MATRIX 'B2' FROM .CSV FILE");

  minMatrix_print_properties(B2);

  MinMatrix C = minMatrix_copy(B);
  minMatrix_print(C, 0, "MATRIX 'C' IS A COPY OF MATRIX 'B'");

  MinMatrix D = minMatrix_get_col(A, 1);
  minMatrix_print(D, 0, "COLUMN 1 FROM MATRIX 'A'");

  MinMatrix E = minMatrix_get_last_col(A);
  minMatrix_print(E, 0, "LAST COLUMN FROM MATRIX 'A'");

  MinMatrix F = minMatrix_multiply(A, B);
  minMatrix_print(F, 0, "MATRIX 'A' * MATRIX 'B'");

  MinMatrix G = minMatrix_identity(5);
  minMatrix_print(G, 0, "MATRIX IDENTITY");

  MinMatrix H = minMatrix_transpose(A);
  minMatrix_print(H, 0, "MATRIX 'A' TRANSPOSED");

  MinMatrix I = minMatrix_minor(A, 0, 0);
  minMatrix_print(I, 0, "MINOR MATRIX FROM MATRIX 'A' AT ROW 0 AND COLUMN 0");

  double det = minMatrix_determinant(A);
  puts("\nDETERMINANT OF MATRIX 'A'");
  puts("-------------------------");
  printf("%.0lf\n\n", det);

  MinMatrix K = minMatrix_cofactor(A);
  minMatrix_print(K, 0, "COFACTOR OF MATRIX 'A'");

  MinMatrix J = minMatrix_inverse(A);
  minMatrix_print(J, 15, "INVERSE OF MATRIX 'A'");

  minMatrix_add_row(B);
  minMatrix_print(B, 0, "ADD ROW TO MATRIX 'B'");

  minMatrix_add_col(B);
  minMatrix_print(B, 0, "ADD COLUMN TO MATRIX 'B'");

  /************************* Benchmark *************************/
  unsigned int iter_benck = 100000;
  clock_t time_begin = clock();
  for (size_t i = 0; i < iter_benck; i++) {
    J = minMatrix_inverse(A);
  }
  clock_t time_end = clock();
  printf(">> Time to compute %ux the inverse matrix: %lfs <<\n\n", iter_benck,
         (double)(time_end - time_begin) / CLOCKS_PER_SEC);
  /*************************************************************/

  minMatrix_destroy(A);
  minMatrix_destroy(B);
  minMatrix_destroy(B2);
  minMatrix_destroy(C);
  minMatrix_destroy(D);
  minMatrix_destroy(E);
  minMatrix_destroy(F);
  minMatrix_destroy(G);
  minMatrix_destroy(H);
  minMatrix_destroy(I);
  minMatrix_destroy(J);
  minMatrix_destroy(K);

  // printf("Number of created matrices..: %u\n", debug_created_matrices);
  // printf("Number of Destroyed matrices: %u\n", debug_destroyed_matrices);

  return 0;
}
