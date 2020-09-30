#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "api.h"



class Matrix
{
private:
  int rows;
  int cols;

public:
  double** array;

  Matrix(int, int);
  ~Matrix()
  {
    delete[] array;
  };
  Matrix operator *(Matrix &second)
  {
    Matrix newMat(rows,second.cols);

    if(cols == second.rows)
    {
      for(int i = 0; i < rows; i++)
      {
        for(int j = 0; j < second.cols; j++)
        {
          for(int k = 0; k < cols; k++)
          {
            newMat.array[i][j] += array[i][k] * second.array[k][i];
          }
        }
      }
    }

    return newMat;
  };

};



#endif
