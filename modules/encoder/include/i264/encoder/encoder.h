/**
 * @file encoder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_ENCODER_INCLUDE_I264_ENCODER_ENCODER_H_
#define MODULES_ENCODER_INCLUDE_I264_ENCODER_ENCODER_H_

#include <list>

#include "i264/common/access_unit.h"
#include "i264/common/frame.h"
#include "i264/encoder/encoder_cfg.h"

namespace i264 {
class RateControl;
class GOPEncoder;
class BitStreamWriter;

class Encoder {
 public:
  explicit Encoder(const EncoderCfg& cfg);
  void Encode(const Frame& frame, std::list<AccessUnit>& bit_stream);

  BitStreamWriter& GetBitStreamWriter();
  RateControl& GetRateControl();

 private:
  const EncoderCfg& cfg_;
  // std::unique_ptr<GOPEncoder> gop_encoder_;
  // std::unique_ptr<RateControl> rate_control_;
  // std::unique_ptr<BitStreamWriter> stream_writer_;
};
}  // namespace i264

#endif  // MODULES_ENCODER_INCLUDE_I264_ENCODER_ENCODER_H_
