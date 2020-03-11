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

#include <list>

#include "i264/common/access_unit.h"
namespace i264 {
class Encoder {
 public:
  explicit Encoder(const EncoderCfg& cfg);
  void Encode(const Frame& frame, std::list<AccessUnit>& bit_stream);

 private:
  std::unique_ptr<GOPEncoder> gop_encoder_;
  std::unique_ptr<RateControl> rate_control_;
  std::unique_ptr<BitStreamWriter> stream_writer_;
};
}  // namespace i264
