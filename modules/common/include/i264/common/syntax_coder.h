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

#include "i264/common/variable_length_coder.h"

namespace i264 {
class SyntaxCoder {
 public:
  static void CodeSPS(const SPS& sps, BitStreamWriter& bitstream_writer);
  static void CodePPS(const PPS& pps, BitStreamWriter& bitstream_writer);
};
}  // namespace i264
