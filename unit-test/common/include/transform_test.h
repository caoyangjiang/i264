// Copyright 2020 Caoyang Jiang

#ifndef UNIT_TEST_COMMON_INCLUDE_TRANFORM_TEST_H_
#define UNIT_TEST_COMMON_INCLUDE_TRANFORM_TEST_H_

WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
WINDOWS_ENABLE_ALL_WARNING

class TransformTest : public ::testing::Test {
 protected:
  TransformTest();
  ~TransformTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_COMMON_INCLUDE_TRANFORM_TEST_H_
