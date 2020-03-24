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

TEST_F(ScaleQuantizeTest, TransformScaleQuantLumaDC) {
  for (int qp = 0; qp < 52; qp += 12) {
    i264::Array2D<int32_t, 16, 16> coefficients;
    i264::Transform::Forward4x4Residual(kX2_, coefficients);
    // std::cout << i264::FormatPrintableArray2D(coefficients) << std::endl;

    i264::Array2D<int32_t, 16, 16> ac_levels;
    i264::QuantizeScale::Quantize(coefficients, qp, ac_levels, true);

    i264::Array2D<int32_t, 4, 4> dc_coefficients;
    i264::Transform::Forward4x4LumaDC(coefficients, dc_coefficients);
    // std::cout << i264::FormatPrintableArray2D(dc_coefficients) << std::endl;

    i264::Array2D<int32_t, 4, 4> dc_levels;
    i264::QuantizeScale::QuantizeLumaDC(dc_coefficients, qp, dc_levels);
    // std::cout << i264::FormatPrintableArray2D(dc_levels) << std::endl;

    i264::Array2D<int32_t, 4, 4> recon_dc_coefficients;
    i264::QuantizeScale::DequantizeLumaDC(dc_levels, qp, recon_dc_coefficients);

    i264::Array2D<int32_t, 16, 16> ac_recon_coefficients;
    i264::QuantizeScale::Dequantize(ac_levels, qp, ac_recon_coefficients, true);

    i264::Array2D<int32_t, 16, 16> recon_dc = ac_recon_coefficients;
    i264::Transform::Inverse4x4LumaDC(recon_dc_coefficients, recon_dc);

    i264::Array2D<int32_t, 16, 16> recon_x2;
    i264::Transform::Inverse4x4Residual(recon_dc, recon_x2);
    // std::cout << i264::FormatPrintableArray2D(recon_x2) << std::endl;

    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        ASSERT_EQ(recon_x2[i][j], kReconResults2_[qp / 12][i % 4][j % 4]);
      }
    }
  }
}

TEST_F(ScaleQuantizeTest, TransformScaleQuantChromaDC) {
  for (int qp = 0; qp < 52; qp += 12) {
    // std::cout << "QP: " << qp << std::endl;
    i264::Array2D<int32_t, 8, 8> coefficients;
    i264::Transform::Forward4x4Residual(kX3_, coefficients);
    // std::cout << i264::FormatPrintableArray2D(coefficients) << std::endl;

    i264::Array2D<int32_t, 8, 8> ac_levels;
    i264::QuantizeScale::Quantize(coefficients, qp, ac_levels, true);
    // std::cout << "AC  " << std::endl;
    // std::cout << i264::FormatPrintableArray2D(ac_levels) << std::endl;

    i264::Array2D<int32_t, 2, 2> dc_coefficients;
    i264::Transform::Forward2x2ChromaDC(coefficients, dc_coefficients);
    // std::cout << i264::FormatPrintableArray2D(dc_coefficients) << std::endl;

    i264::Array2D<int32_t, 2, 2> dc_levels;
    i264::QuantizeScale::QuantizeChromaDC(dc_coefficients, qp, dc_levels);
    // std::cout << i264::FormatPrintableArray2D(dc_levels) << std::endl;

    // std::cout << "DC  " << std::endl;
    // std::cout << i264::FormatPrintableArray2D(dc_levels) << std::endl;

    i264::Array2D<int32_t, 2, 2> recon_dc_coefficients;
    i264::QuantizeScale::DequantizeChromaDC(dc_levels, qp,
                                            recon_dc_coefficients);

    i264::Array2D<int32_t, 8, 8> ac_recon_coefficients;
    i264::QuantizeScale::Dequantize(ac_levels, qp, ac_recon_coefficients, true);

    i264::Array2D<int32_t, 8, 8> recon_coefficients = ac_recon_coefficients;
    i264::Transform::Inverse2x2ChromaDC(recon_dc_coefficients,
                                        recon_coefficients);

    i264::Array2D<int32_t, 8, 8> recon_x3;
    i264::Transform::Inverse4x4Residual(recon_coefficients, recon_x3);
    // std::cout << "RECON " << std::endl
    // std::cout << i264::FormatPrintableArray2D(recon_x3) << std::endl;

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        ASSERT_EQ(recon_x3[i][j], kReconResults3_[qp / 12][i % 4][j % 4]);
      }
    }
  }
}
