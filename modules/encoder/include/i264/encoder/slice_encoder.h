/**
 * @file slice_encoder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <memory>

namespace i264 {
class SliceEncoder {
 public:
  explicit SliceEncoder(FrameEncoder& frame_encoder);

  Coder& GetCoder();
  const Coder& GetCoder() const;

 private:
  std::unique_ptr<Coder> coder_;
};
}  // namespace i264
