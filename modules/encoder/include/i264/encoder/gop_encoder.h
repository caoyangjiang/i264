/**
 * @file gop_encoder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_ENCODER_INCLUDE_I264_ENCODER_GOP_ENCODER_H_
#define MODULES_ENCODER_INCLUDE_I264_ENCODER_GOP_ENCODER_H_

#include <list>
#include <memory>

#include "i264/common/bits.h"
#include "i264/common/frame.h"
#include "i264/common/types.h"

namespace i264 {
class Encoder;

class GOPEncoder {
 public:
  explicit GOPEncoder(Encoder& encoder);
  bool Encode(int poc, const Frame& frame, BitStream& bit_stream);
  bool GetGOPSize() const;
  const std::list<Frame>& GetReconstructedFrames() const;

 private:
  NalUnitType GetNalUnitType(int current_poc);

 private:
  Encoder& encoder_;
  // std::unique_ptr<FrameEncoder> slice_encoder_;
  std::list<Frame> recon_list_;
};
}  // namespace i264
#endif  // MODULES_ENCODER_INCLUDE_I264_ENCODER_GOP_ENCODER_H_
