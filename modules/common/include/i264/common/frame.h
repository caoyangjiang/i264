// Copyright 2020 Caoyang Jiang

#include <array>
#include <cstdint>
#include <vector>

namespace i264 {

class Frame
{
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
  std::array<uint8_t, 3> plane_;
};
}
