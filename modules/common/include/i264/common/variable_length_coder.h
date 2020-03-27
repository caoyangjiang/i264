/**
 * @file variable_length_coder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-24
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_VARIABLE_LENGTH_CODER_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_VARIABLE_LENGTH_CODER_H_

#include "i264/common/bit_stream.h"

namespace i264 {

/**
 * @brief All entropy coding scheme are defined in 7.2
 * and
 *
 */
class VlcCoder {
 public:
  static void CodeAsUE(uint32_t value, uint32_t& exp_golomb_code,
                       uint32_t& length);
  static void CodeAsSE(int value, uint32_t& exp_golomb_code, uint32_t& length);
  static void CodeAsTE(uint32_t value, bool is_binary_range,
                       uint32_t& exp_golomb_code, uint32_t& length);
  static void CodeAsME(int is_chroma_array_type_1_or_2, int is_intra,
                       int coded_block_pattern, uint32_t& exp_golomb_code,
                       uint32_t& length);
  static void CodeAsB8(uint8_t value, uint32_t& output, uint32_t& length);

  // static void CodeAsU(int n, int value, BitStreamWriter& bitstream);
  // static void CodeAsU1(int value, BitStreamWriter& bitstream);
};

}  // namespace i264
#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_VARIABLE_LENGTH_CODER_H_
