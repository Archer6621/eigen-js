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
    std::map<int, Tensor<bool, 2>> adjacencies;
    Tensor<bool, 2> childMask;
    Tensor<bool, 1> leafMask;
    int width;
    int height;
    int tileChoices;

public:
    Grid(int w, int h, int c, DenseMatrix<bool> &adjN, DenseMatrix<bool> &adjE, DenseMatrix<bool> &adjS, DenseMatrix<bool> &adjW, DenseMatrix<bool> &CM, DenseMatrix<bool> &LM)
    {
        choices = Tensor<bool, 3>(w, h, c).setConstant(true);
        adjacencies[0] = TensorCast(adjN.data, c, c);
        adjacencies[1] = TensorCast(adjE.data, c, c);
        adjacencies[2] = TensorCast(adjS.data, c, c);
        adjacencies[3] = TensorCast(adjW.data, c, c);
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
        for(auto dir : {0, 1, 2, 3}) { 
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


    Tensor<bool, 1> getCol(int x, int y) {
        return choices.chip(x, 0).chip(y, 0);
    }

          // Should do prop from ox/oy to nx/ny
    void propagate(int ox, int oy, int nx, int ny, int dir) {
        choices.chip(nx, 0);
        // Tensor<bool, 1> allowed = getCol(ox, oy);
        // Tensor<bool, 1> rem = leafMask && allowed;
        // Tensor<bool, 1> res(tileChoices);
        // res.setConstant(false); // Might be redundant??

        // for (int i = 0; i < this->tileChoices; i++) {
        //   if (rem(i)) {
        //     Tensor<bool, 1> tileAdj = adjacencies.at(dir).chip(i, 0);
        //     for (int j = 0; j < this->tileChoices; j++) {
        //       if (tileAdj(j)) {
        //         res(j) = true;
        //         break;
        //       }
        //     }
        //   }
        // }

        // // Do some other crap


        // // Set the tile choices
        // getCol(nx, ny) = getCol(nx, ny) && res;
      };
};

#endif // GRID_H