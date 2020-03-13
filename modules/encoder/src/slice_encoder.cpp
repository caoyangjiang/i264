/**
 * @file slice_encoder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

namespace i264 {

class SliceEncoder {
 public:
  explicit SliceEncoder(FrameEncoder& frame_encoder);
  bool Encode(Slice& slice);
};

}  // namespace i264
