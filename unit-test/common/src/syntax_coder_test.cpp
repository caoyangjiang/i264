/**
 * @file syntax_coder_test.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "syntax_coder_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/syntax_coder.h"
#include "i264/common/utils.h"

SyntaxCoderTest::SyntaxCoderTest() = default;

SyntaxCoderTest::~SyntaxCoderTest() = default;

void SyntaxCoderTest::SetUp() {}

void SyntaxCoderTest::TearDown() {}

TEST_F(SyntaxCoderTest, SimpleSPS) {
  i264::SPS sps;

  sps.profile_idc = 100;
  sps.level_idc = 10;
  sps.chroma_format_idc = 1;
  sps.pic_order_cnt_type = 0;
  sps.log2_max_pic_order_cnt_lsb_minus4 = 2;
  sps.max_num_ref_frames = 4;
  sps.frame_mbs_only_flag = 1;
  sps.direct_8x8_inference_flag = 1;
  sps.frame_cropping_flag = 1;
  sps.frame_crop_right_offset = 6;
  sps.frame_crop_bottom_offset = 6;

  i264::BitStreamWriter writer;
  i264::SyntaxCoder::CodeSPS(sps, writer);

  auto bit_stream = writer.GetBitStream();

  std::vector<uint8_t> rbsp;
  i264::SyntaxCoder::CodeNalUnit(0x03, 0x07, bit_stream.Data(),
                                 bit_stream.BufferElementCount(), rbsp);

  std::array<uint8_t, 9> reference_out = {0x67, 0x64, 0x00, 0x0a, 0xac,
                                          0xd9, 0x5f, 0x9e, 0x74};
  ASSERT_EQ(rbsp.size(), reference_out.size());
  for (size_t i = 0; i < reference_out.size(); i++) {
    ASSERT_EQ(rbsp[i], reference_out[i]);
  }

  //   i264::WriteNalUnitToFileAsAnnexB("sps.h264", rbsp);
}

TEST_F(SyntaxCoderTest, SimplePPS) {
  i264::PPS pps;

  pps.entropy_coding_mode_flag = 1;
  pps.num_ref_idx_l0_default_active_minus1 = 2;
  pps.weighted_pred_flag = 1;
  pps.weighted_bipred_idc = 2;
  pps.pic_init_qp_minus26 = -20;
  pps.chroma_qp_index_offset = -2;
  pps.deblocking_filter_control_present_flag = 1;
  pps.transform_8x8_mode_flag = 1;
  pps.pic_scaling_matrix_present_flag = 0;
  pps.second_chroma_qp_index_offset = -2;

  i264::BitStreamWriter writer;
  i264::SyntaxCoder::CodePPS(pps, writer);

  auto bit_stream = writer.GetBitStream();

  std::vector<uint8_t> rbsp;
  i264::SyntaxCoder::CodeNalUnit(0x03, 0x08, bit_stream.Data(),
                                 bit_stream.BufferElementCount(), rbsp);

  std::array<uint8_t, 6> reference_out = {0x68, 0xeb, 0xe0, 0x53, 0x2c, 0x8b};
  ASSERT_EQ(rbsp.size(), reference_out.size());
  for (size_t i = 0; i < reference_out.size(); i++) {
    ASSERT_EQ(rbsp[i], reference_out[i]);
  }

  // i264::WriteNalUnitToFileAsAnnexB("pps.h264", rbsp);
}
