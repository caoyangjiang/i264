/**
 * @file transform.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_TRANSFORM_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_TRANSFORM_H_

#include "i264/common/types.h"

namespace i264 {

class Transform {
 public:
  /**
   * @brief This applies to residual of all 4x4 block
   *  ouptut = Cf * input * transpose(Cf)
   *  where Cf is
   *  1  1  1  1
   *  2  1 -1 -2
   *  1 -1 -1  1
   *  1 -2  2 -1
   *
   *  (note no scaling, quantizing of any sort is implemented here.)
   *
   * @tparam BLOCK_WIDTH Input block width. Must be multiple of 4.
   * @tparam BLOCK_HEIGHT Input block height. Must be multiple of 4.
   * @param input Input residual block
   * @param output DCT transformed block
   */
  template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
  static void Forward4x4Residual(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& input,
      Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& output);

  /**
   * @brief This applies to the DCT-transformed coefficients of a intra 16x16
   * macroblock for Luma component only.
   *
   *  ouptut = H * input * H, (H is symmetrical)
   *  where H is
   *  1  1  1  1
   *  1  1 -1 -1
   *  1 -1 -1  1
   *  1 -1  1 -1
   *
   * @param first_trans_coeffs dct coefficients from the initial 4x4 transform
   * @param second_trans_dc_coeff hadamard transformed coefficients.
   */
  static void Forward4x4LumaDC(
      const Array2D<int32_t, 16, 16>& first_trans_coeffs,
      Array2D<int32_t, 4, 4>& second_trans_dc_coeff);

  /**
   * @brief This applies to the DCT-transformed coefficients for Chroma
   * component only.
   *
   *  ouptut = H * input * H, (H is symmetrical)
   *  where H is
   *  1  1
   *  1 -1
   *
   * @param first_trans_coeffs dct coefficients from the initial 4x4 transform
   * @param second_trans_dc_coeff hadamard tranformed coefficients.
   */
  static void Forward2x2ChromaDC(
      const Array2D<int32_t, 8, 8>& first_trans_coeffs,
      Array2D<int32_t, 2, 2>& second_trans_dc_coeff);

  /**
   * @brief This applies to dct-transformed coefficients of all 4x4 block
   * *  ouptut = transpose(Ci) * input * Ci
   *  where Cf is
   *  1   1     1   1
   *  1  1/2  -1/2 -1
   *  1   -1    -1  1
   *  1/2 -1    1 -1/2
   *
   *  (note no scaling, quantizing of any sort is implemented here.)
   *
   * @tparam BLOCK_WIDTH Input block width. Must be multiple of 4.
   * @tparam BLOCK_HEGIHT Input block height. Must be multiple of 4.
   *
   * @param first_trans_coeffs dct-transformed coefficients block
   * @param residual residual block
   */
  template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
  static void Inverse4x4Residual(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& first_trans_coeffs,
      Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& residual);

  /**
   * @brief  This applies to the hadamard-transformed DC coefficients of a intra
   * 16x16 macroblock for luma component only.
   *
   *  ouptut = H * input * H, (H is symmetrical)
   *  where H is
   *  1  1  1  1
   *  1  1 -1 -1
   *  1 -1 -1  1
   *  1 -1  1 -1
   *
   * @param second_trans_dc_coeff hadamard transformed coefficients.
   * @param first_trans_dc_coeff dct coefficients from the initial 4x4 transform
   */
  static void Inverse4x4LumaDC(
      const Array2D<int32_t, 4, 4>& second_trans_dc_coeff,
      Array2D<int32_t, 16, 16>& first_trans_dc_coeff);

  /**
   * @brief This applies to the hadamard-transformed DC coefficients for chroma
   * component only.
   *
   *  ouptut = H * input * H, (H is symmetrical)
   *  where H is
   *  1  1
   *  1 -1
   *
   * @param second_trans_dc_coeff hadamard tranformed coefficients.
   * @param first_trans_dc_coeff dct coefficients from the initial 4x4 transform
   */
  static void Inverse2x2ChromaDC(
      const Array2D<int32_t, 2, 2>& second_trans_dc_coeff,
      Array2D<int32_t, 8, 8>& first_trans_dc_coeff);
};

template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
void Transform::Forward4x4Residual(
    const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& input,
    Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& output) {
  static_assert(BLOCK_WIDTH % 4 == 0, "Block width is not a multiple of 4 ");
  static_assert(BLOCK_HEIGHT % 4 == 0, "Block height is not a multiple of 4 ");

  int block_count_x = BLOCK_WIDTH / 4;
  int block_count_y = BLOCK_HEIGHT / 4;

  for (int blk_y = 0; blk_y < block_count_y; blk_y++) {
    for (int blk_x = 0; blk_x < block_count_x; blk_x++) {
      // transform for this 4x4 starts
      int pixel_x = blk_x * 4;
      int pixel_y = blk_y * 4;

      // horizontal transform
      // compute all e's
      Array2D<int32_t, 4, 4> e;
      for (int i = 0; i < 4; i++) {
        e[0][i] =
            input[pixel_y + 0][pixel_x + i] + input[pixel_y + 3][pixel_x + i];
        e[1][i] =
            input[pixel_y + 0][pixel_x + i] - input[pixel_y + 3][pixel_x + i];
        e[2][i] =
            input[pixel_y + 1][pixel_x + i] + input[pixel_y + 2][pixel_x + i];
        e[3][i] =
            input[pixel_y + 1][pixel_x + i] - input[pixel_y + 2][pixel_x + i];
      }

      // compute all f's
      Array2D<int32_t, 4, 4> f;
      for (int i = 0; i < 4; i++) {
        f[0][i] = e[0][i] + e[2][i];
        f[1][i] = (e[1][i] << 1) + e[3][i];
        f[2][i] = e[0][i] - e[2][i];
        f[3][i] = e[1][i] - (e[3][i] << 1);
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
        output[pixel_y + i][pixel_x + 0] = g[i][0] + g[i][2];
        output[pixel_y + i][pixel_x + 1] = (g[i][1] << 1) + g[i][3];
        output[pixel_y + i][pixel_x + 2] = g[i][0] - g[i][2];
        output[pixel_y + i][pixel_x + 3] = g[i][1] - (g[i][3] << 1);
      }
    }
  }
}

template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
void Transform::Inverse4x4Residual(
    const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& input,
    Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& output) {
  static_assert(BLOCK_WIDTH % 4 == 0, "Block width is not a multiple of 4 ");
  static_assert(BLOCK_HEIGHT % 4 == 0, "Block height is not a multiple of 4 ");

  int block_count_x = BLOCK_WIDTH / 4;
  int block_count_y = BLOCK_HEIGHT / 4;

  for (int blk_y = 0; blk_y < block_count_y; blk_y++) {
    for (int blk_x = 0; blk_x < block_count_x; blk_x++) {
      // transform for this 4x4 starts
      int pixel_x = blk_x * 4;
      int pixel_y = blk_y * 4;

      // horizontal transform
      // compute all e's
      Array2D<int32_t, 4, 4> e;
      for (int i = 0; i < 4; i++) {
        e[0][i] =
            input[pixel_y + 0][pixel_x + i] + input[pixel_y + 2][pixel_x + i];
        e[1][i] =
            input[pixel_y + 0][pixel_x + i] - input[pixel_y + 2][pixel_x + i];
        e[2][i] = (input[pixel_y + 1][pixel_x + i] >> 1) -
                  input[pixel_y + 3][pixel_x + i];
        e[3][i] = input[pixel_y + 1][pixel_x + i] +
                  (input[pixel_y + 3][pixel_x + i] >> 1);
      }

      // compute all f's
      Array2D<int32_t, 4, 4> f;
      for (int i = 0; i < 4; i++) {
        f[0][i] = e[0][i] + e[3][i];
        f[1][i] = e[1][i] + e[2][i];
        f[2][i] = e[1][i] - e[2][i];
        f[3][i] = e[0][i] - e[3][i];
      }

      // vertical transform
      // compute all g's
      Array2D<int32_t, 4, 4> g;
      for (int i = 0; i < 4; i++) {
        g[i][0] = f[i][0] + f[i][2];
        g[i][1] = f[i][0] - f[i][2];
        g[i][2] = (f[i][1] >> 1) - f[i][3];
        g[i][3] = f[i][1] + (f[i][3] >> 1);
      }

      // compute all h's
      Array2D<int32_t, 4, 4> h;
      for (int i = 0; i < 4; i++) {
        h[i][0] = g[i][0] + g[i][3];
        h[i][1] = g[i][1] + g[i][2];
        h[i][2] = g[i][1] - g[i][2];
        h[i][3] = g[i][0] - g[i][3];
      }

      // assign coefficents
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          output[pixel_y + i][pixel_x + j] = h[i][j];
        }
      }

      // for (int i = 0; i < 4; i++) {
      //   for (int j = 0; j < 4; j++) {
      //     output[pixel_y + i][pixel_x + j] = (h[i][j] + (1 << 5)) >> 6;
      //   }
      // }
    }
  }
}

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
    output[i][0] = g[i][0] + g[i][2];
    output[i][1] = g[i][1] + g[i][3];
    output[i][2] = g[i][0] - g[i][2];
    output[i][3] = g[i][1] - g[i][3];
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

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_TRANSFORM_H_
