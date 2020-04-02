/**
 * @file syntax_coder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-25
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <vector>

#include "i264/common/bit_stream.h"
#include "i264/common/parameter_set.h"
#include "i264/common/variable_length_coder.h"

namespace i264 {
class SyntaxCoder {
 public:
  static void CodeSPS(const SPS& sps, BitStreamWriter& bit_stream_writer);
  static void CodePPS(const PPS& pps, BitStreamWriter& bit_stream_writer);
  static void CodeRbspTraillingBits(BitStreamWriter& bit_stream_writer);
  static void CodeRbspSliceTrailingBits(BitStreamWriter& bit_stream_writer,
                                        uint32_t entropy_coding_mode_flag);

  static void CodeNalUnit(uint8_t nal_ref_idc, uint8_t nal_unity_type,
                          const uint8_t* rbsp_bytes, size_t rbs_bytes_len,
                          std::vector<uint8_t>& nal_unit);
};
}  // namespace i264
