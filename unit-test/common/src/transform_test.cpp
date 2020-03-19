
#include "transform_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/quantize.h"
#include "i264/common/transform.h"
#include "i264/common/utils.h"

TransformTest::TransformTest() = default;

TransformTest::~TransformTest() = default;

void TransformTest::SetUp() {}

void TransformTest::TearDown() {}

TEST_F(TransformTest, Simple4x4) {
  i264::Array2D<int32_t, 4, 4> x = {1, 1, 1, 1, 2, 2, 2, 2,
                                    3, 3, 3, 3, 4, 4, 4, 4};

  i264::Array2D<int32_t, 4, 4> output;
  i264::Array2D<int32_t, 4, 4> matlab_result = {40, 0, 0, 0, -28, 0, 0, 0,
                                                0,  0, 0, 0, -4,  0, 0, 0};

  i264::Transform::Forward4x4Residual(x, output);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQ(output[i][j], matlab_result[i][j]);
    }
  }
}

TEST_F(TransformTest, Simple8x8) {
  i264::Array2D<int32_t, 8, 8> x = {
      1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
      3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
      2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

  i264::Array2D<int32_t, 8, 8> output;
  i264::Array2D<int32_t, 4, 4> matlab_result = {40, 0, 0, 0, -28, 0, 0, 0,
                                                0,  0, 0, 0, -4,  0, 0, 0};

  i264::Transform::Forward4x4Residual(x, output);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      ASSERT_EQ(output[i][j], matlab_result[i % 4][j % 4]);
    }
  }
}

TEST_F(TransformTest, Simple16x16) {
  i264::Array2D<int32_t, 16, 16> x = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

  i264::Array2D<int32_t, 16, 16> output;
  i264::Array2D<int32_t, 4, 4> matlab_result = {40, 0, 0, 0, -28, 0, 0, 0,
                                                0,  0, 0, 0, -4,  0, 0, 0};

  i264::Transform::Forward4x4Residual(x, output);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      ASSERT_EQ(output[i][j], matlab_result[i % 4][j % 4]);
    }
  }
}
