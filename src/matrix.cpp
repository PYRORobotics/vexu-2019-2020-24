#include "matrix.h"

Matrix::Matrix(int rows, int cols)
{
  array = new double*[rows];
  for(int i = 0; i < rows; i++)
  {
    array[i] = new double[cols];
    for(int j = 0; j < cols; j++)
      array[i][j] = 0;
  }
}
