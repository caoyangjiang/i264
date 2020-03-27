/**
 * @file variable_length_coder_test.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "variable_length_coder_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <cmath>
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/variable_length_coder.h"

VariableLengthCoderTest::VariableLengthCoderTest() = default;

VariableLengthCoderTest::~VariableLengthCoderTest() = default;

void VariableLengthCoderTest::SetUp() {}

void VariableLengthCoderTest::TearDown() {}

TEST_F(VariableLengthCoderTest, SimpleUE63) {
  constexpr std::array<uint32_t, 63> kLength = {
      1,  3,  3,  5,  5,  5,  5,  7,  7,  7,  7,  7,  7,  7,  7,  9,
      9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
  for (uint32_t i = 0; i < 63; i++) {
    uint32_t code = 0;
    uint32_t length = 0;
    i264::VlcCoder::CodeAsUE(i, code, length);
    ASSERT_EQ(code, i + 1);
    ASSERT_EQ(length, kLength[i]);
  }
}

TEST_F(VariableLengthCoderTest, SimpleSE63) {
  constexpr std::array<uint32_t, 63> kLength = {
      1,  3,  3,  5,  5,  5,  5,  7,  7,  7,  7,  7,  7,  7,  7,  9,
      9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};

  for (int32_t i = 0; i < 63; i++) {
    // Creating 0,1,-1,2,-2,....
    int32_t value =
        (i % 2 == 0 ? -1 : 1) * static_cast<int32_t>(std::ceil(i / 2.0f));
    uint32_t code = 0;
    uint32_t length = 0;
    i264::VlcCoder::CodeAsSE(value, code, length);
    ASSERT_EQ(code, i + 1);
    ASSERT_EQ(length, kLength[i]);
  }
}

TEST_F(VariableLengthCoderTest, SimpleTE63) {
  constexpr std::array<uint32_t, 63> kLength = {
      1,  3,  3,  5,  5,  5,  5,  7,  7,  7,  7,  7,  7,  7,  7,  9,
      9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};

  for (uint32_t i = 0; i < 63; i++) {
    uint32_t code = 0;
    uint32_t length = 0;
    i264::VlcCoder::CodeAsTE(i, false, code, length);
    ASSERT_EQ(code, i + 1);
    ASSERT_EQ(length, kLength[i]);
  }

  // if binary range
  for (uint32_t i = 0; i <= 1; i++) {
    uint32_t code = 0;
    uint32_t length = 0;
    i264::VlcCoder::CodeAsTE(i, true, code, length);
    ASSERT_EQ(code, i == 0 ? 1 : 0);
    ASSERT_EQ(length, 1);
  }
}
