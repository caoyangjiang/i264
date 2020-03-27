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

#include <type_traits>

#include "i264/common/bit_stream.h"

namespace i264 {

/**
 * @brief All entropy coding scheme are defined in 7.2
 *
 *
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

  template <class T, class = std::enable_if_t<std::is_unsigned<T>::value, void>>
  inline static void CodeAsUN(T value, uint32_t value_length, uint32_t& code,
                              uint32_t& code_length) {
    code = static_cast<uint32_t>(value);
    code_length = value_length;
  }

  template <class T, class = std::enable_if_t<std::is_unsigned<T>::value, void>>
  inline static void CodeAsU1(T value, uint32_t& code) {
    code = static_cast<uint32_t>(value & 0x1);
  }

  template <class T>
  inline static void CodeAsF1(T value, uint32_t& code) {
    CodeAsU1(value, code);
  }
};

}  // namespace i264
#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_VARIABLE_LENGTH_CODER_H_
