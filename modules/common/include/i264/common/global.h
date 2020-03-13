/**
 * @file global.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_GLOBAL_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_GLOBAL_H_

#include <cstdint>

namespace i264 {

struct CodedBlockPattern {
  int64_t block;
  int64_t bits;
  int64_t bits_8x8;
};

#define MAX_COLOR_PLANE 3

}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_GLOBAL_H_
