#include "s21_matrix.h"

matrix_t s21_create_matrix(int rows, int columns) {
  matrix_t res;
  res.matrix_type = ZERO_MATRIX;
  res.matrix = (double **)malloc(sizeof(double *) * rows);
  res.rows = rows;
  res.columns = columns;
  for (int i = 0; i < rows; i++) {
    res.matrix[i] = (double *)malloc(columns * sizeof(double *));
  }
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) res.matrix[i][j] = 0.0;

  if (!rows && !columns) res.matrix_type = INCORRECT_MATRIX;
  if (rows < 0 || columns < 0) res.matrix_type = INCORRECT_MATRIX;
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
  free(A->matrix);
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int flag = SUCCESS;
  if (A->columns == B->columns && A->rows == B->rows) {
    for (int i = 0; i < A->rows && flag != FAILURE; i++) {
      for (int j = 0; j < A->columns && flag != FAILURE; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >
            0.0000001 * fabs(B->matrix[i][j])) {
          flag = FAILURE;
        }
      }
    }
  } else {
    flag = FAILURE;
  }
  return flag;
}

matrix_t s21_sum_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  res = s21_create_matrix(A->rows, A->columns);
  if (A->columns == B->columns && A->rows == B->rows) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        res.matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    value_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_sub_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  res = s21_create_matrix(A->rows, A->columns);
  if (A->columns == B->columns && A->rows == B->rows) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        res.matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    value_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_mult_number(matrix_t *A, double number) {
  matrix_t res;
  res = s21_create_matrix(A->rows, A->columns);
  for (int i = 0; i < A->rows; i++)
    for (int j = 0; j < A->columns; j++)
      res.matrix[i][j] = A->matrix[i][j] * number;
  value_type(&res);
  return res;
}

matrix_t s21_mult_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  res = s21_create_matrix(A->rows, B->columns);
  if (A->columns == B->rows) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->rows; j++) {
        res.matrix[i][j] = 0;
        for (int k = 0; k < B->rows; k++)
          res.matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
    value_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_transpose(matrix_t *A) {
  matrix_t res;
  res = s21_create_matrix(A->columns, A->rows);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      res.matrix[j][i] = A->matrix[i][j];
    }
  }
  value_type(&res);
  return res;
}

matrix_t s21_calc_complements(matrix_t *A) {
  matrix_t res;
  res = s21_create_matrix(A->columns, A->rows);
  if (A->columns == A->rows && A->rows != 1) {
    matrix_t tmp;
    tmp = s21_create_matrix(A->rows - 1, A->rows - 1);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->rows; j++) {
        cut_out_matrix(A, &tmp, i, j);
        res.matrix[i][j] = pow(-1, 2 + i + j) * determinant(&tmp);
      }
    }
    s21_remove_matrix(&tmp);
    value_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

double s21_determinant(matrix_t *A) {
  double res = 0;
  if (A->rows == A->columns)
    res = determinant(A);
  else
    res = NAN;
  return res;
}

matrix_t s21_inverse_matrix(matrix_t *A) {
  matrix_t res;
  res = s21_create_matrix(A->rows, A->columns);
  if (A->rows == A->columns && A->rows > 1) {
    double determ = s21_determinant(A);
    if (determ != 0.0) {
      determ = 1.0 / determ;
      matrix_t tmp1, tmp2, tmp3;
      tmp1 = s21_calc_complements(A);
      tmp2 = s21_transpose(&tmp1);
      tmp3 = s21_mult_number(&tmp2, determ);
      for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
          res.matrix[i][j] = tmp3.matrix[i][j];
      s21_remove_matrix(&tmp1);
      s21_remove_matrix(&tmp2);
      s21_remove_matrix(&tmp3);
      value_type(&res);
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

void value_type(matrix_t *A) {
  matrix_t tmp;
  tmp = s21_create_matrix(A->rows, A->columns);
  A->matrix_type = ZERO_MATRIX;
  if (s21_eq_matrix(A, &tmp) == 1) {
    A->matrix_type = ZERO_MATRIX;
  } else if (A->rows == A->columns && identity(A)) {
    A->matrix_type = IDENTITY_MATRIX;
  } else {
    A->matrix_type = CORRECT_MATRIX;
  }
  s21_remove_matrix(&tmp);
}

int identity(matrix_t *A) {
  int flag = 1;
  for (int i = 0; i < A->rows; i++) {
    if (A->matrix[i][i] != 1) {
      flag = 0;
      break;
    }
  }
  return flag;
}

void cut_out_matrix(matrix_t *A, matrix_t *tmp, int i, int j) {
  int row_tmp = 0;
  for (int i1 = 0; i1 < A->rows - 1; i1++) {
    int col_tmp;
    col_tmp = 0;
    if (i1 == i) row_tmp = 1;
    for (int j1 = 0; j1 < A->rows - 1; j1++) {
      if (j1 == j) col_tmp = 1;
      tmp->matrix[i1][j1] = A->matrix[i1 + row_tmp][j1 + col_tmp];
    }
  }
}

double determinant(matrix_t *A) {
  matrix_t tmp;
  tmp = s21_create_matrix(A->rows, A->columns);
  double res = 0;
  int tmp_row = A->rows - 1;
  if (A->rows == 1) {
    res = A->matrix[0][0];
  } else if (A->rows == 2) {
    res = A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else {
    int sign = 1;
    for (int i = 0; i < A->rows; i++) {
      cut_out_matrix(A, &tmp, i, 0);
      tmp.rows = tmp_row;
      res += sign * A->matrix[i][0] * determinant(&tmp);
      sign *= -1;
    }
  }
  tmp.rows = A->rows;
  s21_remove_matrix(&tmp);
  return res;
}
