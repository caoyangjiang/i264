/**
 * @file mb_encoder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/encoder/macroblock_encoder.h"

#include "i264/common/types.h"
#include "i264/encoder/slice_encoder.h"

namespace i264 {

MacroblockEncoder::MacroblockEncoder(const SliceEncoder& slice_encoder)
    : slice_encoder_(slice_encoder) {}

void MacroblockEncoder::Encode(Macroblock& macroblock) {
  // if this block is a intra block
  //   TryIntra16x16Prediction(macroblock, int& cost16x16, enum
  //   INTRA_16x16_MODE& best_i16x16_mode);

  //   TryIntra4x4Prediction(macroblock, int& cost4x4, INTRA_4x4_MODE&
  //   best_i4x4_mode[16]);

  // choose the best prediction type and modes
  // save to macroblock

  // Code this macroblock
  // Done
}

}  // namespace i264
