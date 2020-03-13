/**
 * @file frame.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_FRAME_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_FRAME_H_

#include <array>
#include <cstdint>
#include <vector>

namespace i264 {

enum class ColorModel { UNKNOWN = -1, YUV = 0, RGB = 1, XYZ = 2 };

enum class ColorFormat {
  UNKNOWN = -1,
  YUV400 = 0,
  YUV420 = 1,
  YUV422 = 2,
  YUV444 = 3
};

class FrameBuffer {
 public:
  FrameBuffer() = default;
  ~FrameBuffer() = default;

  FrameBuffer(int width, int height, int channel);

  uint8_t* Buffer(int channel);
  const uint8_t* Buffer(int channel) const;
  int Width(int channel) const;
  int Height(int channel) const;

 private:
  std::shared_ptr<uint8_t[]> buffer_;
};

class Frame {
 public:
 private:
  int picture_order_count = 0;
  int type_ = 0;
  int forced_type_ = 0;
  int qp_plus_1_ = 0;
  int64_t pts_ = 0;
  int64_t dts_ = 0;
  int64_t reordered_pts_ = 0;
  int64_t duration_ = 0;
  float duration_sec_ = 0.0f;
  int64_t cpb_duration_;
  int64_t cpb_delay_;
  int64_t dpb_output_delay;

  int present_frame_number_ = 0;
  int coded_frame_number_ = 0;
  int frame_num_ = 0;
  bool kept_as_ref = false;
  int pic_struct;
  bool key_frame_ = true;
  bool b_fdec;
  float qp_avg_rc;
  float qp_avg_aq;

  std::array<int, 3> stride_;
  std::array<int, 3> width_;
  std::array<int, 3> height_;
  std::array<int, 3> lines_;

  std::vector<uint8_t> img_buffer_;
  std::array<uint8_t*, 3> plane_;
};
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_FRAME_H_
