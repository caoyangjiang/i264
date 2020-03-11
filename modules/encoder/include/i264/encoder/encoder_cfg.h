/**
 * @file encoder_cfg.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

namespace i264 {

class EncoderCfg {
 public:
  EncoderCfg() = default;
  ~EncoderCfg() = default;

  // Setters
  inline void SetProfileIDC(int idc) { profile_idc_ = idc; }
  inline void SetLevelIDC(int idc) { level_idc_ = idc; }
  inline void SetIntraProfile(int profile) { intra_profile_ = profile; }
  inline void SetNumberOfFrameToSkip(int skip) { frame_skip_ = skip; }
  inline void SetQPForSliceTypes(int qp, int slice_type) {
    qp_slice_type_[slice_type] = qp;
  }
  inline void SetYuvFormat(int format) { yuv_format_ = format; }
  inline void SetSliceMode(int mode) { slice_mode_ = mode; }
  inline void SetIDRPeriod(int period) { idr_period_ = period; }
  inline void SetIntraPeriod(int period) { intra_period_ = period; }
  inline void EnableDeblockingFilter() { disable_deblocking_filter_ = false; }
  inline void DisableDeblockingFilter() { disable_deblocking_filter_ = true; }
  inline void EnableIntra4x4() { disable_intra_4x4_ = false; }
  inline void DisableIntra4x4() { disable_intra_4x4_ = true; }
  inline void EnableIntra8x8() { disable_intra_8x8_ = false; }
  inline void DisableIntra8x8() { disable_intra_8x8_ = true; }
  inline void EnableFastIntra16x16() { fast_intra_16x16_ = true; }
  inline void DisableFastIntra16x16() { fast_intra_16x16_ = false; }
  inline void EnableFastIntra8x8() { fast_intra_8x8_ = true; }
  inline void DisableFastIntra8x8() { fast_intra_8x8_ = false; }
  inline void EnableFastIntra4x4() { fast_intra_4x4_ = true; }
  inline void DisableFastIntra4x4() { fast_intra_4x4_ = false; }
  inline void SetFrameRate(double rate) { frame_rate_ = rate; }
  inline void SetChromaQPIndexOffset(int offset) {
    chroma_qp_index_offset_ = offset;
  }
  inline void SetPicOrderCountType(int type) { pic_order_cnt_type_ = type; }
  inline void EnableTransform8x8Mode() { transform_8x8_mode_ = true; }
  inline void DisableTransform8x8Mode() { transform_8x8_mode_ = false; }
  inline void EnableRateControl() { rate_control_enable_ = true; }
  inline void DisableRateControl() { rate_control_enable_ = false; }
  inline void SetRCBitRate(int bits_per_sec) { bit_rate_ = bits_per_sec; }
  inline void SetRCMinQP(int min_qp, int slice_type) {
    rate_control_min_qp_[slice_type] = min_qp;
  }
  inline void SetRCMaxQP(int max_qp, int slice_type) {
    rate_control_max_qp_[slice_type] = max_qp;
  }

  // Getters
  inline int GetProfileIDC() { return profile_idc_; }
  inline int GetLevelIDC() { return level_idc_; }
  inline int GetIntraProfile() { return intra_profile_; }
  inline int GetNumberOfFrameToSkip() { return frame_skip_; }
  inline int GetQPForSliceTypes(int qp, int slice_type) {
    if (slice_type < 5)
      return qp_slice_type_[slice_type];
    else
      return -1;
  }
  inline int GetYuvFormat() { return yuv_format_; }
  inline int GetSliceMode() { return slice_mode_; }
  inline int GetIDRPeriod() { return idr_period_; }
  inline int GetIntraPeriod() { return intra_period_; }
  inline bool IsDeblockingFilterEnabled() {
    return !disable_deblocking_filter_;
  }
  inline bool IsIntra4x4Enabled() { return !disable_intra_4x4_; }
  inline bool IsEnableIntra8x8Enabled() { return !disable_intra_8x8_; }
  inline bool IsFastIntra16x16Enabled() { return fast_intra_16x16_; }
  inline bool IsFastIntra8x8Enabled() { return fast_intra_8x8_; }
  inline bool IsFastIntra4x4Enabled() { return fast_intra_4x4_; }
  inline double GetFrameRate() { return frame_rate_; }
  inline int GetChromaQPIndexOffset() { return chroma_qp_index_offset_; }
  inline int GetPicOrderCountType() { return pic_order_cnt_type_; }
  inline bool IsTransform8x8ModeEnabled() { return transform_8x8_mode_; }
  inline bool IsRateControlEnabled() { return rate_control_enable_; }
  inline int GetRCBitRate() { return bit_rate_; }
  inline int GetRCMinQP(int slice_type) {
    if (slice_type < 5)
      return rate_control_min_qp_[slice_type];
    else
      return -1;
  }

  inline int GetRCMaxQP(int slice_type) {
    if (slice_type < 5)
      return rate_control_max_qp_[slice_type];
    else
      return -1;
  }

 private:
  int profile_idc_ = 0;
  int level_idc_ = 0;
  int intra_profile_ = 0;

  int frame_skip_ = 0;

  std::array<int, 5> qp_slice_type_ = {0, 0, 0, 0, 0};

  int yuv_format_ = 1;  // 0: YUV400, 1: YUV420
  int slice_mode_ = 0;

  int idr_period_ = 8;
  int intra_period_ = 8;

  bool disable_deblocking_filter_ = true;

  bool disable_intra_4x4_ = true;
  bool disable_intra_8x8_ = true;
  bool fast_intra_16x16_ = false;
  bool fast_intra_8x8_ = false;
  bool fast_intra_4x4_ = false;

  double frame_rate_ = 30.0f;
  int chroma_qp_index_offset_ = 0;
  int pic_order_cnt_type_ = 0;
  bool transform_8x8_mode_ = false;

  // rate control
  int rate_control_enable_;
  int bit_rate_;
  std::array<int, 5> rate_control_min_qp_ = {0, 0, 0, 0, 0};
  std::array<int, 5> rate_control_max_qp_ = {51, 51, 51, 51, 51};
};
}  // namespace i264
