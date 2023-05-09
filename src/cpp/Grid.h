#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <string>
#include <unsupported/Eigen/CXX11/Tensor>
#include "DenseMatrix.h"
#include "Conversion.h"
#include <map>

using namespace Eigen;
using namespace std;

class Grid
{
    //   using Mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    private:
        Tensor<bool, 3> choices;
        std::map<char,  Tensor<bool, 2> > adjacencies;
        Tensor<bool, 2> childMask;
        Tensor<bool, 1> leafMask;

    public:
      Grid(int w, int h, int tileChoices, DenseMatrix<bool>& adjN, DenseMatrix<bool>& adjE, DenseMatrix<bool>& adjS,DenseMatrix<bool>& adjW, DenseMatrix<bool>& CM, DenseMatrix<bool>& LM)
      {
          choices = Tensor<bool, 3>(w, h, tileChoices).setConstant(true);
          adjacencies['N'] = TensorCast(adjN.data, tileChoices, tileChoices);
          adjacencies['E'] = TensorCast(adjE.data, tileChoices, tileChoices);
          adjacencies['S'] = TensorCast(adjS.data, tileChoices, tileChoices);
          adjacencies['W'] = TensorCast(adjW.data, tileChoices, tileChoices);
          childMask = TensorCast(CM.data, tileChoices, tileChoices);
          leafMask = TensorCast(LM.data, tileChoices);
      }




};




#endif // GRID_H