/**
 * @file utils.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-04-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/utils.h"

namespace i264 {
std::string FormatPrintableBitStream(const BitStream& bit_stream) {
  std::ostringstream os;
  for (size_t i = 0; i < bit_stream.Size(); i++) {
    os << static_cast<uint32_t>(bit_stream[i]) << " ";
  }
  os << '\n';

  return os.str();
}
}  // namespace i264
