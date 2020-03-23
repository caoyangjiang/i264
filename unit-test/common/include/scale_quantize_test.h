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

class ScaleQuantizeTest : public ::testing::Test {
 protected:
  ScaleQuantizeTest();
  ~ScaleQuantizeTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_COMMON_INCLUDE_SCALE_QUANTIZE_TEST_H_
