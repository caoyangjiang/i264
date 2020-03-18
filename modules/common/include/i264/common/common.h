// Copyright 2020 Caoyang Jiang

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_COMMON_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_COMMON_H_

WINDOWS_DISABLE_ALL_WARNING
#include <cstdint>
WINDOWS_ENABLE_ALL_WARNING

namespace i264 {

class I264_COMMON_DLL Test {
 public:
  Test() = default;
  ~Test() = default;

  int Run() const;
};

template <class T>
struct Position {
  T x;
  T y;
};

constexpr int BIT_DEPTH = 8;
constexpr uint32_t PIXEL_MAX_VALUE = (1 << BIT_DEPTH) - 1;
constexpr uint32_t PIXEL_MIN_VALUE = 0;
constexpr int MB_SIZE = 16;
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_COMMON_H_
