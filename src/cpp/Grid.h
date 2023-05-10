#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <string>
#include <unsupported/Eigen/CXX11/Tensor>
#include "DenseMatrix.h"
#include "Conversion.h"
#include <map>
#include <initializer_list>

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
    // std::array<std::string, 3> texts = ;
    // // ^ An array of 3 elements with the type std::string
        for(auto dir : {'N', 'E', 'S', 'W'}) { 
            printf("%c\n", dir);
            for (int i = 0; i < this->tileChoices; i++)
            {
                printf(i == 0 ? "[[" : " [");
                for (int j = 0; j < this->tileChoices; j++)
                {
                    printf("%d", this->adjacencies.at(dir)(i,j));
                    printf(j < this->tileChoices - 1 ? ", " : "");
                }
                printf((i < this->tileChoices - 1 ? "]" : "]]"));
                printf("\n");
            }
        }
    }
};

#endif // GRID_H