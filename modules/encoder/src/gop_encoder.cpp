/**
 * @file gop_encoder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/encoder/gop_encoder.h"

#include "i264/common/types.h"
#include "i264/encoder/encoder.h"

namespace i264 {

GOPEncoder::GOPEncoder(Encoder& encoder) : encoder_(encoder) {
  // initialize frame encoder
}

bool GOPEncoder::Encode(int poc, const Frame& frame, BitStream& bit_stream) {
  // determine this frame nalu type

  // configure frame encoder for this frame

  // compress this frame with frame encoder

  return true;
}

}  // namespace i264
