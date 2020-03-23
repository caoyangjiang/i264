/**
 * @file scale_quantize_test.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef UNIT_TEST_COMMON_INCLUDE_SCALE_QUANTIZE_TEST_H_
#define UNIT_TEST_COMMON_INCLUDE_SCALE_QUANTIZE_TEST_H_

WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/types.h"

class ScaleQuantizeTest : public ::testing::Test {
 protected:
  ScaleQuantizeTest();
  ~ScaleQuantizeTest() override;

  void SetUp() override;
  void TearDown() override;

  static constexpr i264::Array2D<int32_t, 4, 4> kX_ = {
      58, 64, 51, 58, 52, 64, 56, 66, 62, 63, 61, 64, 59, 51, 63, 69};

  static constexpr std::array<i264::Array2D<int32_t, 4, 4>, 5> kLevelResults_ =
      {// QP=0,
       384, -10, 6, -12, -8, 12, -7, -17, -6, 1, 6, 6, 3, 13, -1, 1,
       // QP=12,
       96, -3, 1, -3, -2, 3, -2, -4, -1, 0, 1, 1, 1, 3, 0, 0,
       // QP=24,
       24, -1, 0, -1, -1, 1, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0,
       // QP=36,
       6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       // QP=48,
       2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  static constexpr std::array<i264::Array2D<int32_t, 4, 4>, 5> kReconResults_ =
      {// QP=0,
       58, 64, 51, 58, 52, 64, 56, 66, 62, 63, 61, 64, 59, 51, 63, 69,
       // QP=12,
       57, 65, 52, 59, 52, 63, 57, 65, 62, 62, 61, 65, 58, 52, 64, 69,
       // QP=24,
       56, 65, 48, 58, 51, 61, 56, 66, 60, 62, 61, 64, 54, 58, 69, 72,
       // QP=36,
       60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
       // QP=48,
       80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
};

#endif  // UNIT_TEST_COMMON_INCLUDE_SCALE_QUANTIZE_TEST_H_
