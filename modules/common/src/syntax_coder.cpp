/**
 * @file syntax_coder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/syntax_coder.h"

#include <cmath>

#include "i264/common/bit_stream.h"
#include "i264/common/variable_length_coder.h"

namespace i264 {

void SyntaxCoder::CodeSPS(const SPS& sps, BitStreamWriter& bit_stream_writer) {
  uint32_t code = 0, code_length = 0;
  VlcCoder::CodeAsUN(sps.profile_idc, 8, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(sps.constrained_set0_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.constrained_set1_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.constrained_set2_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.constrained_set3_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.constrained_set4_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.constrained_set5_flag, code);
  bit_stream_writer.Write(code, 1);

  uint32_t reserved_zero_2bits = 0;
  bit_stream_writer.Write(reserved_zero_2bits, 2);

  VlcCoder::CodeAsUN(sps.level_idc, 8, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsUE(sps.seq_parameter_set_id, code, code_length);
  bit_stream_writer.Write(code, code_length);

  if ((sps.profile_idc == 100) || (sps.profile_idc == 110) ||
      (sps.profile_idc == 122) || (sps.profile_idc == 244) ||
      (sps.profile_idc == 44) || (sps.profile_idc == 83) ||
      (sps.profile_idc == 86) || (sps.profile_idc == 118) ||
      (sps.profile_idc == 128) || (sps.profile_idc == 138) ||
      (sps.profile_idc == 139) || (sps.profile_idc == 134)) {
    VlcCoder::CodeAsUE(sps.chroma_format_idc, code, code_length);
    bit_stream_writer.Write(code, code_length);

    if (sps.chroma_format_idc == 3) {
      VlcCoder::CodeAsU1(sps.separate_colour_plane_flag, code);
      bit_stream_writer.Write(code, 1);
    }

    VlcCoder::CodeAsUE(sps.bit_depth_luma_minus8, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsUE(sps.bit_depth_chroma_minus8, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsU1(sps.qpprime_y_zero_transform_bypass_flag, code);
    bit_stream_writer.Write(code, 1);

    VlcCoder::CodeAsU1(sps.seq_scaling_matrix_present_flag, code);
    bit_stream_writer.Write(code, 1);

    if (sps.seq_scaling_matrix_present_flag) {
      // scaling list not implemented.
    }
  }  // end sps.profile_idc check

  VlcCoder::CodeAsUE(sps.log2_max_frame_num_minus4, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsUE(sps.pic_order_cnt_type, code, code_length);
  bit_stream_writer.Write(code, code_length);

  if (sps.pic_order_cnt_type == 0) {
    VlcCoder::CodeAsUE(sps.log2_max_pic_order_cnt_lsb_minus4, code,
                       code_length);
    bit_stream_writer.Write(code, code_length);
  } else if (sps.pic_order_cnt_type == 1) {
    VlcCoder::CodeAsU1(sps.delta_pic_order_always_zero_flag, code);
    bit_stream_writer.Write(code, 1);

    VlcCoder::CodeAsSE(sps.offset_for_non_ref_pic, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsSE(sps.offset_for_top_to_bottom_field, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsUE(sps.num_ref_frames_in_pic_order_cnt_cycle, code,
                       code_length);
    bit_stream_writer.Write(code, code_length);

    for (uint32_t i = 0; i < sps.num_ref_frames_in_pic_order_cnt_cycle; i++) {
      VlcCoder::CodeAsSE(sps.offset_for_ref_frame[i], code, code_length);
      bit_stream_writer.Write(code, code_length);
    }
  }

  VlcCoder::CodeAsUE(sps.max_num_ref_frames, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(sps.gaps_in_frame_num_value_allowed_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsUE(sps.pic_width_in_mbs_minus1, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsUE(sps.pic_height_in_map_units_minus1, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(sps.frame_mbs_only_flag, code);
  bit_stream_writer.Write(code, 1);

  if (!sps.frame_mbs_only_flag) {
    VlcCoder::CodeAsU1(sps.mb_adaptive_frame_field_flag, code);
    bit_stream_writer.Write(code, 1);
  }

  VlcCoder::CodeAsU1(sps.direct_8x8_inference_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(sps.frame_cropping_flag, code);
  bit_stream_writer.Write(code, 1);

  if (sps.frame_cropping_flag) {
    VlcCoder::CodeAsUE(sps.frame_crop_left_offset, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsUE(sps.frame_crop_right_offset, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsUE(sps.frame_crop_top_offset, code, code_length);
    bit_stream_writer.Write(code, code_length);

    VlcCoder::CodeAsUE(sps.frame_crop_bottom_offset, code, code_length);
    bit_stream_writer.Write(code, code_length);
  }

  uint32_t vui_parameters_present_flag =
      sps.vui_parameters_.has_value() ? 1 : 0;
  VlcCoder::CodeAsU1(vui_parameters_present_flag, code);
  bit_stream_writer.Write(code, 1);

  if (vui_parameters_present_flag) {
    // VUI parameters not implemented yet.
  }

  CodeRbspTraillingBits(bit_stream_writer);
}

void SyntaxCoder::CodePPS(const PPS& pps, BitStreamWriter& bit_stream_writer) {
  uint32_t code = 0, code_length = 0;

  VlcCoder::CodeAsUE(pps.pic_parameter_set_id, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsUE(pps.seq_parameter_set_id, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(pps.entropy_coding_mode_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(pps.bottom_field_pic_order_in_frame_present_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsUE(pps.num_slice_groups_minus1, code, code_length);
  bit_stream_writer.Write(code, code_length);

  //   if (pps.num_slice_groups_minus1 > 0) {
  //     VlcCoder::CodeAsUE(pps.slice_group_map_type, code, code_length);
  //     bit_stream_writer.Write(code, code_length);

  //     if (pps.slice_group_map_type == 0) {
  //       for (uint32_t igroup = 0; igroup <= pps.num_slice_groups_minus1;
  //            igroup++) {
  //         VlcCoder::CodeAsUE(pps.run_length_minus1[igroup], code,
  //         code_length); bit_stream_writer.Write(code, code_length);
  //       }
  //     } else if (pps.slice_group_map_type == 2) {
  //       for (uint32_t igroup = 0; igroup < pps.num_slice_groups_minus1;
  //            igroup++) {
  //         VlcCoder::CodeAsUE(pps.top_left[igroup], code, code_length);
  //         bit_stream_writer.Write(code, code_length);
  //         VlcCoder::CodeAsUE(pps.bottom_right[igroup], code, code_length);
  //         bit_stream_writer.Write(code, code_length);
  //       }
  //     } else if (pps.slice_group_map_type == 3 || pps.slice_group_map_type ==
  //     4 ||
  //                pps.slice_group_map_type == 5) {
  //       VlcCoder::CodeAsU1(pps.slice_group_change_direction_flag, code);
  //       bit_stream_writer.Write(code, 1);
  //       VlcCoder::CodeAsUE(pps.slice_group_change_rate_minus1, code,
  //       code_length); bit_stream_writer.Write(code, code_length);
  //     } else if (pps.slice_group_map_type == 6) {
  //       VlcCoder::CodeAsUE(pps.pic_size_in_map_units_minus1, code,
  //       code_length); bit_stream_writer.Write(code, code_length);

  //       uint32_t slice_group_id_length =
  //           std::ceil(std::log2(pps.num_slice_groups_minus1 + 1));
  //       for (uint32_t i = 0; i < pps.pic_size_in_map_units_minus1; i++) {
  //         VlcCoder::CodeAsUN(pps.pic_size_in_map_units_minus1,
  //                            slice_group_id_length, code, code_length);
  //         bit_stream_writer.Write(code, code_length);
  //       }
  //     }
  //   }

  VlcCoder::CodeAsUE(pps.num_ref_idx_l0_default_active_minus1, code,
                     code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsUE(pps.num_ref_idx_l1_default_active_minus1, code,
                     code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(pps.weighted_pred_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsUN(pps.weighted_bipred_idc, 2, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsSE(pps.pic_init_qp_minus26, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsSE(pps.pic_init_qs_minus26, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsSE(pps.chroma_qp_index_offset, code, code_length);
  bit_stream_writer.Write(code, code_length);

  VlcCoder::CodeAsU1(pps.deblocking_filter_control_present_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(pps.constrained_intra_pred_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(pps.redundant_pic_cnt_present_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(pps.transform_8x8_mode_flag, code);
  bit_stream_writer.Write(code, 1);

  VlcCoder::CodeAsU1(pps.pic_scaling_matrix_present_flag, code);
  bit_stream_writer.Write(code, 1);

  if (pps.pic_scaling_matrix_present_flag) {
    // not implemented yet
  }

  VlcCoder::CodeAsSE(pps.second_chroma_qp_index_offset, code, code_length);
  bit_stream_writer.Write(code, code_length);

  CodeRbspTraillingBits(bit_stream_writer);
}

void SyntaxCoder::CodeRbspTraillingBits(BitStreamWriter& bit_stream_writer) {
  bit_stream_writer.WriteOne();
  while (bit_stream_writer.GetNumberOfBitsUntilByteAligned() != 0) {
    bit_stream_writer.WriteZero();
  }
}

void SyntaxCoder::CodeRbspSliceTrailingBits(BitStreamWriter& bit_stream_writer,
                                            uint32_t entropy_coding_mode_flag) {
  // 7.3.2.10
  CodeRbspTraillingBits(bit_stream_writer);
  if (entropy_coding_mode_flag) {
    // not implemented
  }
}

void SyntaxCoder::CodeNalUnit(uint8_t nal_ref_idc, uint8_t nal_unit_type,
                              const uint8_t* nal_unit, size_t nal_unit_len,
                              std::vector<uint8_t>& rbsp_bytes) {
  rbsp_bytes.reserve(rbsp_bytes.size() * 3 / 2);
  rbsp_bytes.push_back(((0x03 & nal_ref_idc) << 5) + (0x1F & nal_unit_type));

  // if(nal_unit_type == 14 || nal_unit_type==20 || nal_unit_type == 21)
  // { not implemented }

  int leading_zero_count = 0;
  for (uint32_t i = 0; i < nal_unit_len; i++) {
    if ((leading_zero_count == 2) && (nal_unit[i] <= 0x03)) {
      rbsp_bytes.push_back(0x03);
      leading_zero_count = 0;
    }

    if (nal_unit[i] == 0) {
      leading_zero_count++;
    } else {
      leading_zero_count = 0;
    }

    rbsp_bytes.push_back(nal_unit[i]);
  }
}

}  // namespace i264
