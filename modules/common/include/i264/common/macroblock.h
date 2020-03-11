/**
 * @file macroblock.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_

#include "i264/common/common.h"
#include "i264/common/global.h"

namespace i264 {
class Macroblock {
 public:
  /**
   * @brief Pick a 4x4 to perform the inverse 4x4 transformation
   *
   * @param plane plane id
   * @param x_offset horizontal offset to the 4x4 block within the macroblock.
   * @param y_offset vertical pixel offset to the 4x4 block within the
   * macroblock
   */
  void InverseTransform4x4(ColorPlane plane, int x_offset, int y_offset);

  /**
   * @brief Pick a 8x8 to perform the inverse 8x8 transformation
   *
   * @param plane plane id
   * @param x_offset horizontal offset to the 4x4 block within the macroblock.
   * @param y_offset vertical pixel offset to the 4x4 block within the
   * macroblock
   */
  void InverseTransform4x4(ColorPlane plane, int x_offset, int y_offset);

 private:
  Slice* slice_ = std::nullptr_t;
  int address_zscan_ = -1;
  Position<uint16_t> pos_2d_ = {0, 0};
  uint16_t type_ = 0;

  // coding
  int cbp_;
  std::array<CodedBitPattern, 3> cbp_struct_;
  int intra16_mode_ = 0;
  uint8_t chroma_intra_prediction_mode_ = 0;

  // subblock access
  int block_x = 0;
  int block_y = 0;
  int block_y_aff = 0;
  int pix_x = 0;
  int pix_y = 0;
  int pix_c_x = 0;
  int pix_c_y = 0;
  int subblock_x = 0;
  int subblock_y = 0;

  // quantize related
  std::array<int, MAX_COLOR_PLANE> qp_;
  std::array<int, MAX_COLOR_PLANE> qp_scale_;
  bool is_lossless = false;
  bool is_intra_block = true;

  uint8_t intra_pred_modes[16];
  uint8_t intra_pred_modes8x8[16];

  // neighbor information
  Macroblock* neighbor_a = std::nullptr_t;
  Macroblock* neighbor_b = std::nullptr_t;
  Macroblock* neighbor_c = std::nullptr_t;
  Macroblock* neighbor_d = std::nullptr_t;
};
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_
