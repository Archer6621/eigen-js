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
    std::map<char, Tensor<bool, 2>> adjacencies;
    Tensor<bool, 2> childMask;
    Tensor<bool, 1> leafMask;
    int width;
    int height;
    int tileChoices;

public:
    Grid(int w, int h, int c, DenseMatrix<bool> &adjN, DenseMatrix<bool> &adjE, DenseMatrix<bool> &adjS, DenseMatrix<bool> &adjW, DenseMatrix<bool> &CM, DenseMatrix<bool> &LM)
    {
        choices = Tensor<bool, 3>(w, h, c).setConstant(true);
        adjacencies['N'] = TensorCast(adjN.data, c, c);
        adjacencies['E'] = TensorCast(adjE.data, c, c);
        adjacencies['S'] = TensorCast(adjS.data, c, c);
        adjacencies['W'] = TensorCast(adjW.data, c, c);
        childMask = TensorCast(CM.data, c, c);
        leafMask = TensorCast(LM.data, c);
        width = w;
        height = h;
        tileChoices = c;
    }

    void print() const
    {
        for (int i = 0; i < this->width; i++)
        {
            printf(i == 0 ? "[[" : " [");
            for (int j = 0; j < this->height; j++)
            {
                printf("%d", this->adjacencies.at('N')(i,j));
                printf(j < this->height - 1 ? ", " : "");
            }
            printf((i < this->width - 1 ? "]" : "]]"));
            printf("\n");
        }
    }
};

#endif // GRID_H