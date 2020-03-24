/**
 * @file transform.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/transform.h"

namespace i264 {

void Transform::Forward4x4LumaDC(const Array2D<int32_t, 16, 16>& input,
                                 Array2D<int32_t, 4, 4>& output) {
  // Defined in 8.5.10

  // horizontal transform
  // compute all e's
  Array2D<int32_t, 4, 4> e;
  for (int i = 0; i < 4; i++) {
    e[0][i] = input[0][4 * i] + input[12][4 * i];
    e[1][i] = input[0][4 * i] - input[12][4 * i];
    e[2][i] = input[4][4 * i] + input[8][4 * i];
    e[3][i] = input[4][4 * i] - input[8][4 * i];
  }

  // compute all f's
  Array2D<int32_t, 4, 4> f;
  for (int i = 0; i < 4; i++) {
    f[0][i] = e[0][i] + e[2][i];
    f[1][i] = e[1][i] + e[3][i];
    f[2][i] = e[0][i] - e[2][i];
    f[3][i] = e[1][i] - e[3][i];
  }

  // vertical transform
  // compute all g's
  Array2D<int32_t, 4, 4> g;
  for (int i = 0; i < 4; i++) {
    g[i][0] = f[i][0] + f[i][3];
    g[i][1] = f[i][0] - f[i][3];
    g[i][2] = f[i][1] + f[i][2];
    g[i][3] = f[i][1] - f[i][2];
  }

  // compute all h's
  for (int i = 0; i < 4; i++) {
    output[i][0] = (g[i][0] + g[i][2]) >> 1;
    output[i][1] = (g[i][1] + g[i][3]) >> 1;
    output[i][2] = (g[i][0] - g[i][2]) >> 1;
    output[i][3] = (g[i][1] - g[i][3]) >> 1;
  }
}

void Transform::Inverse4x4LumaDC(const Array2D<int32_t, 4, 4>& input,
                                 Array2D<int32_t, 16, 16>& output) {
  // Defined in 8.5.10

  // horizontal transform
  // compute all e's
  Array2D<int32_t, 4, 4> e;
  for (int i = 0; i < 4; i++) {
    e[0][i] = input[0][i] + input[3][i];
    e[1][i] = input[0][i] - input[3][i];
    e[2][i] = input[1][i] + input[2][i];
    e[3][i] = input[1][i] - input[2][i];
  }

  // compute all f's
  Array2D<int32_t, 4, 4> f;
  for (int i = 0; i < 4; i++) {
    f[0][i] = e[0][i] + e[2][i];
    f[1][i] = e[1][i] + e[3][i];
    f[2][i] = e[0][i] - e[2][i];
    f[3][i] = e[1][i] - e[3][i];
  }

  // vertical transform
  // compute all g's
  Array2D<int32_t, 4, 4> g;
  for (int i = 0; i < 4; i++) {
    g[i][0] = f[i][0] + f[i][3];
    g[i][1] = f[i][0] - f[i][3];
    g[i][2] = f[i][1] + f[i][2];
    g[i][3] = f[i][1] - f[i][2];
  }

  // compute all h's
  Array2D<int32_t, 4, 4> h;
  for (int i = 0; i < 4; i++) {
    h[i][0] = g[i][0] + g[i][2];
    h[i][1] = g[i][1] + g[i][3];
    h[i][2] = g[i][0] - g[i][2];
    h[i][3] = g[i][1] - g[i][3];
  }

  // assign coefficents
  for (int i = 0; i < 4; i++) {
    output[0][4 * i] = h[0][i];
    output[4][4 * i] = h[1][i];
    output[8][4 * i] = h[2][i];
    output[12][4 * i] = h[3][i];
  }
}

void Transform::Forward2x2ChromaDC(const Array2D<int32_t, 8, 8>& input,
                                   Array2D<int32_t, 2, 2>& output) {
  // Defined in 8.5.11.1

  // horizontal transform
  // compute all e's
  Array2D<int32_t, 2, 2> e;
  for (int i = 0; i < 2; i++) {
    e[0][i] = input[0][4 * i] + input[4][4 * i];
    e[1][i] = input[0][4 * i] - input[4][4 * i];
  }

  // vertical transform
  // compute all h's
  for (int i = 0; i < 2; i++) {
    output[i][0] = e[i][0] + e[i][1];
    output[i][1] = e[i][0] - e[i][1];
  }
}

void Transform::Inverse2x2ChromaDC(const Array2D<int32_t, 2, 2>& input,
                                   Array2D<int32_t, 8, 8>& output) {
  // Defined in 8.5.11.1

  // horizontal transform
  // compute all e's
  Array2D<int32_t, 2, 2> e;
  for (int i = 0; i < 2; i++) {
    e[0][i] = input[0][i] + input[1][i];
    e[1][i] = input[0][i] - input[1][i];
  }

  // vertical transform
  // compute all h's
  for (int i = 0; i < 2; i++) {
    output[4 * i][0] = e[i][0] + e[i][1];
    output[4 * i][4] = e[i][0] - e[i][1];
  }
}
}  // namespace i264
