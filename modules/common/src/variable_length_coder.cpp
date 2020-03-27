/**
 * @file variable_length_coder.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/variable_length_coder.h"

#include <iostream>

namespace i264 {

static const Array2D<uint8_t, 48, 2> kCBPtoCodeNumMappingForChromaType12{
    /* intra_4x4 or intra_8x8 (48 total) */
    3, 29, 30, 17, 31, 18, 37, 8, 32, 38, 19, 9, 20, 10, 11, 2, 16, 33, 34, 21,
    35, 22, 39, 4, 36, 40, 23, 5, 24, 6, 7, 1, 41, 42, 43, 25, 44, 26, 46, 12,
    45, 47, 27, 13, 28, 14, 15, 0,
    /* inter (48 total) */
    0, 2, 3, 7, 4, 8, 17, 13, 5, 18, 9, 14, 10, 15, 16, 11, 1, 32, 33, 36, 34,
    37, 44, 40, 35, 45, 38, 41, 39, 42, 43, 19, 6, 24, 25, 20, 26, 21, 46, 28,
    27, 47, 22, 29, 23, 30, 31, 12};

void VlcCoder::CodeAsUE(uint32_t value, uint32_t& exp_golomb_code,
                        uint32_t& code_length) {
  // Defined in 9.1
  uint32_t length = 1;
  uint32_t tmp = ++value;

  while (1 != tmp) {
    tmp >>= 1;
    length += 2;
  }

  code_length = 2 * (length >> 1) + 1;
  exp_golomb_code = value;
}

void VlcCoder::CodeAsSE(int32_t value, uint32_t& exp_golomb_code,
                        uint32_t& code_length) {
  // Defined in 9.1
  uint32_t cvtsyntax = (value <= 0) ? static_cast<uint32_t>(-value << 1)
                                    : static_cast<uint32_t>((value << 1) - 1);
  CodeAsUE(cvtsyntax, exp_golomb_code, code_length);
}

void VlcCoder::CodeAsTE(uint32_t value, bool binary_range,
                        uint32_t& exp_golomb_code, uint32_t& code_length) {
  // Defined in 9.1
  if (binary_range) {
    exp_golomb_code = value == 0 ? 1 : 0;
    code_length = 1;
  } else {
    CodeAsUE(value, exp_golomb_code, code_length);
  }
}

void VlcCoder::CodeAsME(int is_chroma_array_type_1_or_2, int is_intra,
                        int coded_block_pattern, uint32_t& exp_golomb_code,
                        uint32_t& code_length) {
  if (is_chroma_array_type_1_or_2) {
    auto code_num = kCBPtoCodeNumMappingForChromaType12[is_intra ? 0 : 1]
                                                       [coded_block_pattern];
    CodeAsUE(code_num, exp_golomb_code, code_length);
  } else {
    std::cout << "ME coding for chroma array 0 or 3 is not implemented."
              << std::endl;
    assert(0);
  }
}

}  // namespace i264
