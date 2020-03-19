/**
 * @file utils.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_UTILS_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_UTILS_H_

#include <iostream>
#include <sstream>
#include <string>

#include "i264/common/types.h"

namespace i264 {

template <class T, unsigned long ROWS, unsigned long COLUMNS>
std::string FormatPrintableArray2D(const Array2D<T, ROWS, COLUMNS>& array) {
  std::ostringstream os;
  for (unsigned long i = 0; i < ROWS; i++) {
    for (unsigned long j = 0; j < COLUMNS; j++) {
      os << static_cast<int>(array[i][j]) << " ";
    }
    os << '\n';
  }

  return os.str();
}
}  // namespace i264
#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_UTILS_H_
