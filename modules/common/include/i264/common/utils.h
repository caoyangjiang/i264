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

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

template <class T>
void WriteSequenceToFile(const std::string& filename, const T& sequence) {
  std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
  ofs.write(reinterpret_cast<const char*>(sequence.data()), sequence.size());
}

template <class T>
void WriteNalUnitToFileAsAnnexB(const std::string& filename,
                                const T& sequence) {
  std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
  std::vector<uint8_t> start_code = {0x00, 0x00, 0x00, 0x001};
  ofs.write(reinterpret_cast<const char*>(start_code.data()),
            start_code.size());
  ofs.write(reinterpret_cast<const char*>(sequence.data()), sequence.size());
}

}  // namespace i264
#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_UTILS_H_
