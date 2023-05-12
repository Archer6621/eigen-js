#ifndef GRID_H
#define GRID_H

#include <emscripten/bind.h>
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
      // this->choices.imag

    std::cout << this->choices << std::endl;
    // std::array<std::string, 3> texts =
    
     ;
    // // ^ An array of 3 elements with the type std::string
        // for(auto dir : {0, 1, 2, 3}) { 
        //     printf("%c\n", dir);
        //     for (int i = 0; i < this->tileChoices; i++)
        //     {
        //         printf(i == 0 ? "[[" : " [");
        //         for (int j = 0; j < this->tileChoices; j++)
        //         {
        //             printf("%d", this->adjacencies.at(dir)(i,j));
        //             printf(j < this->tileChoices - 1 ? ", " : "");
        //         }
        //         printf((i < this->tileChoices - 1 ? "]" : "]]"));
        //         printf("\n");
        //     }
        // }
    }


    auto getCol(int x, int y) {
        return choices.chip(x, 0).chip(y, 0);
    }



// const pre = squeeze(this.choices.subset(index(nx, ny, this.ALL)));
// const cur = squeeze(this.choices.subset(index(p.x, p.y, this.ALL)));
// let allowedAdjacencies = zeros(this.gridChoices);
// for (const idx of this.indices(cur._data)) {
//   if (this.LM.get([idx])) {
//     let adjacencies = squeeze(
//       this.adjaug.get(k).subset(index(idx, this.ALL))
//     );
//     allowedAdjacencies = add(allowedAdjacencies, adjacencies);
//   }
// }
// const post = and(pre, allowedAdjacencies);

    void setCol(int x, int y, std::vector<double> newCol) {
      Tensor<bool, 1> col = getCol(x, y);
      std::cout << "BEFORE" << std::endl;
      std::cout << col << std::endl;
      for (int i = 0; i < tileChoices; i++) {
        col(i) = newCol[i] != 0.0;
      }
      std::cout << "AFTER" << std::endl;
      std::cout << getCol(x, y) << std::endl;

    }

    bool propagate(int ox, int oy, int nx, int ny, int dir) {
        Tensor<bool, 1> pre = getCol(nx, ny);
        Tensor<bool, 1> cur = getCol(ox, oy);
        // Tensor<bool, 1> rem = leafMask && allowed;
        Tensor<bool, 1> allowedAdjacencies(tileChoices);
        allowedAdjacencies.setConstant(false);
        // res; // Might be redundant??

        for (int i = 0; i < this->tileChoices; i++) {
          if (cur(i) && this->leafMask(i)) {
            Tensor<bool, 1> tileAdj = adjacencies.at(dir).chip(i, 1);
            allowedAdjacencies = allowedAdjacencies || tileAdj;

            // for (int j = 0; j < this->tileChoices; j++) {
            //   if (tileAdj(j)) {
            //     res(j) = true;
            //     break;
            //   }
            // }
          }
        }

        // Do some other crap


        // Set the tile choices
        Tensor<bool, 1> post = pre && allowedAdjacencies;
        getCol(nx, ny) = post; 
        Tensor<bool, 0> diff = (pre ^ post).any();

        std::cout << "DIFF" << std::endl;
        std::cout << "pre" << std::endl;
        std::cout << pre << std::endl;
        std::cout << "post" << std::endl;
        std::cout << post << std::endl;


        // Return the difference
        return diff();
      };

      
      auto getChoices() {


        // std::vector<uint8_t> output = { 1, 0, 1 };

        Tensor<uint8_t, 3> casted = this->choices.cast<uint8_t>();
        auto size = this->width * this->height * this->tileChoices;
        
        emscripten::val view{ emscripten::typed_memory_view(size, casted.data()) };
        auto result = emscripten::val::global("Uint8Array").new_(size);
        result.call<void>("set", view);

        return result;
      }

};

#endif // GRID_H