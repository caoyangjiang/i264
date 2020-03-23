/**
 * @file scale_quantize_test.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "scale_quantize_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/quantize.h"
#include "i264/common/transform.h"
#include "i264/common/utils.h"

ScaleQuantizeTest::ScaleQuantizeTest() = default;

ScaleQuantizeTest::~ScaleQuantizeTest() = default;

void ScaleQuantizeTest::SetUp() {}

void ScaleQuantizeTest::TearDown() {}

TEST_F(ScaleQuantizeTest, TransformScaleQuant4x4) {
  for (int qp = 0; qp < 52; qp += 12) {
    i264::Array2D<int32_t, 4, 4> coefficients;

    i264::Transform::Forward4x4Residual(kX_, coefficients);

    i264::Array2D<int32_t, 4, 4> levels;
    i264::QuantizeScale::Quantize(coefficients, qp, levels, false);

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        ASSERT_EQ(levels[i][j], kLevelResults_[qp / 12][i][j]);
      }
    }

    i264::Array2D<int32_t, 4, 4> recon_coefficients;
    i264::QuantizeScale::Dequantize(levels, qp, recon_coefficients, false);

    i264::Array2D<int32_t, 4, 4> recon_x;
    i264::Transform::Inverse4x4Residual(recon_coefficients, recon_x);

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        ASSERT_EQ(recon_x[i][j], kReconResults_[qp / 12][i][j]);
      }
    }
  }
}

TEST_F(ScaleQuantizeTest, TransformScaleQuant16x16) {
  i264::Array2D<int32_t, 16, 16> x;

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      x[i][j] = kX_[i % 4][j % 4];
    }
  }

  for (int qp = 0; qp < 52; qp += 12) {
    i264::Array2D<int32_t, 16, 16> coefficients;

    i264::Transform::Forward4x4Residual(x, coefficients);

    i264::Array2D<int32_t, 16, 16> levels;
    i264::QuantizeScale::Quantize(coefficients, qp, levels, false);

    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        ASSERT_EQ(levels[i][j], kLevelResults_[qp / 12][i % 4][j % 4]);
      }
    }

    i264::Array2D<int32_t, 16, 16> recon_coefficients;
    i264::QuantizeScale::Dequantize(levels, qp, recon_coefficients, false);

    i264::Array2D<int32_t, 16, 16> recon_x;
    i264::Transform::Inverse4x4Residual(recon_coefficients, recon_x);

    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        ASSERT_EQ(recon_x[i][j], kReconResults_[qp / 12][i % 4][j % 4]);
      }
    }
  }
}
