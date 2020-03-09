/**
 * @file sps.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cstdint>
#include <optional>
#include <vector>

#include "i264/common/types.h"

namespace i264 {

// (self not nal unit, part of SPS)
struct VUIParameters {};

// 7.3.2.1.1 (nal unit type = 7)
struct SPS {
  bool valid = false;  // is this needed?

  uint64_t profile_idc = 0;        // u(8)
  bool constrained_set0_flag = 0;  // u(1)
  bool constrained_set1_flag = 0;  // u(1)
  bool constrained_set2_flag = 0;  // u(1)
  bool constrained_set3_flag = 0;  // u(1)

  uint32_t level_idc = 0;                        // u(8)
  uint32_t seq_parameter_set_id = 0;             // ue(v)
  uint32_t chroma_format_idc = 0;                // ue(v)
  uint32_t separate_colour_plane_flag = 0;       // u(1)
  uint32_t bit_depth_luma_minus8 = 0;            // ue(v)
  uint32_t bit_depth_chroma_minus8 = 0;          // ue(v)
  int qpprime_y_zero_transform_bypass_flag = 0;  // u(1)

  bool seq_scaling_matrix_present_flag = false;       // u(1)
  std::array<int, 12> seq_scaling_list_present_flag;  // u(1)
  Matrix2D<int, 6, 16> scaling_list_4x4;              // se(v)
  Matrix2D<int, 6, 64> scaling_list_8x8;              // se(v)
  std::array<bool, 6> use_default_scaling_matrix_4x4_flag;
  std::array<bool, 6> use_default_scaling_matrix_8x8_flag;

  uint32_t log2_max_frame_num_minus4 = 0;          // ue(v)
  uint32_t pic_order_cnt_type = 0;                 // ue(v)
  uint32_t log2_max_pic_order_cnt_lsb_minus4 = 0;  // ue(v)

  bool delta_pic_order_always_zero_flag = false;       // u(1)
  int offset_for_non_ref_pic = 0;                      // se(v)
  int offset_for_top_to_bottom_field = 0;              // se(v)
  uint32_t num_ref_frames_in_pic_order_cnt_cycle = 0;  // ue(v)
  std::array<int, 256> offset_for_ref_frame;           // se(v)

  uint32_t max_num_ref_frames = 0;                  // ue(v)
  bool gaps_in_frame_num_value_allowed_flag = = 0;  // u(1)
  uint32_t pic_width_in_mbs_minus1 = 0;             // ue(v)
  uint32_t pic_height_in_map_units_minus1 = 0;      // ue(v)
  bool frame_mbs_only_flag = false;                 // u(1)

  bool mb_adaptive_frame_field_flag = false;  // u(1)
  bool direct_8x8_inference_flag = false;     // u(1)
  bool frame_cropping_flag = false;           // u(1)
  uint32_t frame_crop_left_offset = 0;        // ue(v)
  uint32_t frame_crop_right_offset = 0;       // ue(v)
  uint32_t frame_crop_top_offset = 0;         // ue(v)
  uint32_t frame_crop_bottom_offset = 0;      // ue(v)
  std::optional<VUIParameters> vui_parameters_;
};

// 7.3.2.1.2 (nal unit type = 13)
struct SPSExtension {
  uint32_t seq_parameter_set_id = 0;     // ue(v)
  uint32_t aux_format_idc = 0;           // ue(v)
  uint32_t bit_depth_aux_minus8 = 0;     // ue(v)
  bool alpha_incr_flag = 0;              // u(1)
  uint32_t alpha_opaque_value = 0;       // u(v)
  uint32_t alpha_transparent_value = 0;  // ue(v)
  bool additional_extension_flag = 0;    // u(1)
};

// 7.3.2.1.3 (nal unit type = 15. For MVC, SVC)
struct SubsetSPS {};

// 7.3.2.2 (nal unit type = 8)
struct PPS {
  bool valid = false;  // is this needed?

  uint32_t pic_parameter_set_id = 0;                  // ue(v)
  uint32_t seq_parameter_set_id = 0;                  // ue(v)
  bool entropy_coding_mode_flag = false;              // u(1)
  bool bottom_field_pic_order_in_frame_present_flag;  // u(1)
  uint32_t num_slice_groups_minus1 = 0;               // ue(v)
  uint32_t slice_group_map_type = 0;                  // ue(v)
  std::array<int, 8> run_length_minus1;               // ue(v)
  std::array<int, 8> top_left;                        // ue(v)
  std::array<int, 8> bottom_right;                    // ue(v)
  bool slice_group_change_direction_flag = false;     // u(1)
  uint32_t slice_group_change_rate_minus1 = 0;        // ue(v)
  uint32_t pic_size_in_map_units_minus1 = 0;          // ue(v)
  std::vector<uint8_t> slice_group_id;                // ue(v)

  uint32_t num_ref_idx_l0_default_active_minus1 = 0;  // ue(v)
  uint32_t num_ref_idx_l1_default_active_minus1 = 0;  // ue(v)
  bool weighted_pred_flag = false;                    // u(1)
  uint32_t weighted_bipred_idc = 0;                   // u(2)
  int pic_init_qp_minus26 = 0;                        // se(v)
  int pic_init_ps_minus26 = 0;                        // se(v)
  int chroma_qp_index_offset = 0;                     // se(v)
  bool deblocking_filter_control_present_flag = 0;    // u(1)
  bool constrained_intra_pred_flag = 0;               // u(1)
  bool redundant_pic_cnt_present_flag = 0;            // u(1)
  bool transform_8x8_mode_flag = false;               // u(1)
  bool pic_scaling_matrix_present_flag = 0;           // u(1)

  std::array<int, 12> pic_scaling_list_present_flag;  // u(1)
  Matrix2D<int, 6, 16> scaling_list_4x4;              // se(v)
  Matrix2D<int, 6, 64> scaling_list_8x8;              // se(v)
  std::array<bool, 6> use_default_scaling_matrix_4x4_flag;
  std::array<bool, 6> use_default_scaling_matrix_8x8_flag;

  int second_chroma_qp_index_offset;  // se(v)
};
}  // namespace i264
