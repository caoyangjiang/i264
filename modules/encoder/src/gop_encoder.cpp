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

namespace i264 {

GOPEncoder::GOPEncoder(Encoder& encoder) {
  // initialize frame encoder
}

bool GOPEncoder::Encode(int poc, const Frame& frame, Bits& BitStream) {
  // determine this frame nalu type

  // configure frame encoder for this frame

  // compress this frame with frame encoder

  return true;
}

}  // namespace i264
