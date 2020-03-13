/**
 * @file coder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_ENCODER_INCLUDE_I264_ENCODER_CODER_H_
#define MODULES_ENCODER_INCLUDE_I264_ENCODER_CODER_H_

#include "i264/common/types.h"

namespace i264 {
class Slice;

class Coder {
 public:
  void SetBitStream(Bits* output_bitstream);
  const Bits* GetOutputBitStream() const;

  SliceType DetermineCabacInitIndex(const Slice& slice);
  void CodeTerminatingBit(uint32_t ls_last);  // what is ls?
  void CodeSliceFinish();

 private:
  Bits* bits_ = std::nullptr_t;
};
}  // namespace i264

#endif  // MODULES_ENCODER_INCLUDE_I264_ENCODER_CODER_H_
