/**
 * @file quantize.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/common.h"
#include "i264/common/types.h"

namespace i264 {

// the pre-scaling matrix V is defined for QP0-5
// 1. for all other QP > 5, it is derived from this
//    V(QP) = dequant4_scale(QP%6) * 2^(QP/6)
// 2. v is roughly = Si(from idct normalization) * Qstep * 2^6
static const std::array<Array2D<uint8_t, 4, 4>, 6> kDequant4_Scale{
    // Defined in 8.5.9, V is expanded for QP 0-5
    // QP=0
    10, 13, 10, 13, 13, 16, 13, 16, 10, 13, 10, 13, 13, 16, 13, 16,
    // QP=1
    11, 14, 11, 14, 14, 18, 14, 18, 11, 14, 11, 14, 14, 18, 14, 18,
    // QP=2
    13, 16, 13, 16, 16, 20, 16, 20, 13, 16, 13, 16, 16, 20, 16, 20,
    // QP=3
    14, 18, 14, 18, 18, 23, 18, 23, 14, 18, 14, 18, 18, 23, 18, 23,
    // QP=4
    16, 20, 16, 20, 20, 25, 20, 25, 16, 20, 16, 20, 20, 25, 20, 25,
    // QP=5
    18, 23, 18, 23, 23, 29, 23, 29, 18, 23, 18, 23, 23, 29, 23, 29};

// The post-scaling matrix M is not defined in the standard
// but it is doing "inverse" of dequat4_scale.
// 1. for all other QP > 5, it is derived from this
//    M(QP) = quant4_scale(QP%6) / 2^(QP/6)
// 2. There is a relationship between quant4_scale and dequant4_scale
//    quant4_scale = Si(from idct normalization) * Sf(from dct normalization) *
//    2^21 / dequat4_scale;
//    (for detail read "The H.264 Advanced Video Compression Standard 2nd
//    Edition" Section 7.2.3.7)
static const std::array<Array2D<uint16_t, 4, 4>, 6> kQuant4_Scale{
    // QP=0
    /* row0 */ 13107, 8066, 13107, 8066,
    /* row1 */ 8066, 5243, 8066, 5243,
    /* row2 */ 13107, 8066, 13107, 8066,
    /* row3 */ 8066, 5243, 8066, 5243,

    // QP=1
    /* row0 */ 11916, 7490, 11916, 7490,
    /* row1 */ 7490, 4660, 7490, 4660,
    /* row2 */ 11916, 7490, 11916, 7490,
    /* row3 */ 7490, 4660, 7490, 4660,

    // QP=2
    /* row0 */ 10082, 6554, 10082, 6554,
    /* row1 */ 6554, 4194, 6554, 4194,
    /* row2 */ 10082, 6554, 10082, 6554,
    /* row3 */ 6554, 4194, 6554, 4194,

    // QP=3
    /* row0 */ 9362, 5825, 9362, 5825,
    /* row1 */ 5825, 3647, 5825, 3647,
    /* row2 */ 9362, 5825, 9362, 5825,
    /* row3 */ 5825, 3647, 5825, 3647,

    // QP=4
    /* row0 */ 8192, 5243, 8192, 5243,
    /* row1 */ 5243, 3355, 5243, 3355,
    /* row2 */ 8192, 5243, 8192, 5243,
    /* row3 */ 5243, 3355, 5243, 3355,

    // QP=5
    /* row0 */ 7282, 4559, 7282, 4559,
    /* row1 */ 4559, 2893, 4559, 2893,
    /* row2 */ 7282, 4559, 7282, 4559,
    /* row3 */ 4559, 2893, 4559, 2893};

// Defined for syntax "seq_scaling_matrix_present_flag"
// In the standard, when there is no scaling matrix, "sequence level
// scaling list specified by Flat_4x4_16 shall be inferred for i=0..15".
static const Array2D<uint8_t, 4, 4> kFlat_4x4_16{
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

/**
 * @brief Perform quantization and scaling.
 *
 */
class QuantizeScale {
 public:
  template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
  static void Quantize(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients,
      const int qp, Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels,
      bool dc_skip);

  template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
  static void Dequantize(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels, const int qp,
      Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients, bool dc_skip);

  template <int BLOCK_WIDTH, int BLOCK_HEIGHT>
  static void DequantizeLumaDC(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels, const int qp,
      Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients);

  template <int BLOCK_WIDTH, int BLOCK_HEIGHT>
  static void DequantizeChromaDC(
      const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels, const int qp,
      Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients);
};

template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
void QuantizeScale::Dequantize(
    const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels, const int qp,
    Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients, bool dc_skip) {
  static_assert(BLOCK_WIDTH % 4 == 0, "Block width is not a multiple of 4 ");
  static_assert(BLOCK_HEIGHT % 4 == 0, "Block height is not a multiple of 4 ");

  // Defined in 8.5.9
  Array2D<int32_t, 4, 4> level_scale_4x4;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      level_scale_4x4[i][j] =
          static_cast<int32_t>(kDequant4_Scale[qp % 6][i][j]) << 4;
    }
  }

  // Defined in 8.5.12.1
  int block_count_x = BLOCK_WIDTH / 4;
  int block_count_y = BLOCK_HEIGHT / 4;

  for (int blk_y = 0; blk_y < block_count_y; blk_y++) {
    for (int blk_x = 0; blk_x < block_count_x; blk_x++) {
      // quantize for this 4x4 starts
      int pixel_x = blk_x * 4;
      int pixel_y = blk_y * 4;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          // for intra16, dc is skipped
          if (dc_skip && i == 0 && j == 0) continue;

          if (qp >= 24) {
            if (levels[i][j] >= 0)
              coefficients[i][j] = levels[i][j] * level_scale_4x4[i][j]
                                   << (qp / 6 - 4);  // (8-336)
            else
              coefficients[i][j] = -(-levels[i][j] * level_scale_4x4[i][j]
                                     << (qp / 6 - 4));  // (8-336)
          } else {
            if (levels[i][j] >= 0)
              coefficients[i][j] = (levels[i][j] * level_scale_4x4[i][j] +
                                    (1 << (3 - qp / 6))) >>
                                   (4 - qp / 6);  // (8-337)
            else
              coefficients[i][j] = -((-levels[i][j] * level_scale_4x4[i][j] +
                                      (1 << (3 - qp / 6))) >>
                                     (4 - qp / 6));  // (8-337)
          }
        }
      }
    }
  }
}

template <unsigned long BLOCK_WIDTH, unsigned long BLOCK_HEIGHT>
void QuantizeScale::Quantize(
    const Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& coefficients,
    const int qp, Array2D<int32_t, BLOCK_WIDTH, BLOCK_HEIGHT>& levels,
    bool dc_skip) {
  // Defined in 8.5.12.1
  int block_count_x = BLOCK_WIDTH / 4;
  int block_count_y = BLOCK_HEIGHT / 4;

  for (int blk_y = 0; blk_y < block_count_y; blk_y++) {
    for (int blk_x = 0; blk_x < block_count_x; blk_x++) {
      // quantize for this 4x4 starts
      int pixel_x = blk_x * 4;
      int pixel_y = blk_y * 4;

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          // for intra16, dc is skipped
          if (dc_skip && i == 0 && j == 0) continue;

          // There is more than one way to "round" to the integer level
          // Standard does not say about this. Each encoder can implement
          // whatever they want. This does not affect decoding because any
          // sample reused for further encoding is always reconstructed strictly
          // according to the standard.
          //
          // x264 implements a "quant4_bias" for this purpose.
          // here we are use rounding to the nearest integer, which is different
          // from x264 and JM.
          if (coefficients[i][j] >= 0) {
            levels[i][j] = ((coefficients[i][j] * kQuant4_Scale[qp % 6][i][j]) +
                            (1 << (14 + qp / 6))) >>
                           (15 + qp / 6);
          } else {
            levels[i][j] =
                -(((-coefficients[i][j] * kQuant4_Scale[qp % 6][i][j]) +
                   (1 << (14 + qp / 6))) >>
                  (15 + qp / 6));
          }
        }
      }
    }
  }
}

}  // namespace i264
