#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <unsupported/Eigen/CXX11/Tensor>
#include "DenseMatrix.h"
#include "Conversion.h"

using namespace Eigen;
using namespace std;

class Grid
{
    //   using Mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    private:
        Tensor<bool, 3> choices;
        Tensor<bool, 2> adjacencies;
        Tensor<bool, 2> childMask;
        Tensor<bool, 1> leafMask;

    public:
      Grid(int w, int h, int tileChoices, DenseMatrix<bool>& adj, DenseMatrix<bool>& CM, DenseMatrix<bool>& LM)
      {
          choices = Tensor<bool, 3>(w, h, tileChoices).setConstant(true);
          adjacencies = TensorCast(adj.data, tileChoices, tileChoices);
          childMask = TensorCast(CM.data, tileChoices, tileChoices);
          leafMask = TensorCast(LM.data, tileChoices);
      }




};




#endif // GRID_H