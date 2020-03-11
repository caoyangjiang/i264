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

#include <list>
#include <memory>

#include "i264/common/bits.h"
namespace i264 {
class GOPEncoder {
 public:
  explicit GOPEncoder(Encoder& encoder);
  bool Encode(const Frame& frame, Bits& BitStream);
  bool GetGOPSize() const;
  const std::list<Frame>& GetReconstructedFrames() const;

 private:
  Encoder& encoder_;
  std::unique_ptr<SliceEncoder> slice_encoder_;
  std::list<Frame> recon_list_;
};
}  // namespace i264
