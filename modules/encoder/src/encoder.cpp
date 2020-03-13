/**
 * @file encoder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/encoder/encoder.h"

namespace i264 {
Encoder::Encoder(const EncoderCfg& cfg) : cfg_(cfg) {
  // initialize gop encoder

  // initialize rate control

  // ToBeDone: initialize RDO
}

void Encoder::Encode(const Frame& frame, std::list<AccessUnit>& bit_stream) {
  // call gop encoder encode

  // collect statistics
}
}  // namespace i264
