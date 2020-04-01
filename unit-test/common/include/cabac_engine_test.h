// Copyright 2020 Caoyang Jiang

#ifndef UNIT_TEST_COMMON_INCLUDE_CABAC_ENGINE_TEST_H_
#define UNIT_TEST_COMMON_INCLUDE_CABAC_ENGINE_TEST_H_

WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
WINDOWS_ENABLE_ALL_WARNING

class CabacEngineTest : public ::testing::Test {
 protected:
  CabacEngineTest();
  ~CabacEngineTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_COMMON_INCLUDE_CABAC_ENGINE_TEST_H_
