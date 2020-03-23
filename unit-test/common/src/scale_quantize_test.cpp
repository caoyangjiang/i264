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
  for (int qp = 0; qp < 30; qp += 6) {
    std::cout << "QP = " << qp << std::endl;
    // i264::Array2D<int32_t, 4, 4> x = {1, 1, 1, 1, 2, 2, 2, 2,
    //                                   1, 1, 1, 1, 2, 2, 2, 2};
    i264::Array2D<int32_t, 4, 4> x = {58, 64, 51, 58, 52, 64, 56, 66,
                                      62, 63, 61, 64, 59, 51, 63, 69};
    i264::Array2D<int32_t, 4, 4> coefficients;
    i264::Array2D<int32_t, 4, 4> matlab_result = {40, 0, 0, 0, -28, 0, 0, 0,
                                                  0,  0, 0, 0, -4,  0, 0, 0};

    i264::Transform::Forward4x4Residual(x, coefficients);

    i264::Array2D<int32_t, 4, 4> levels;
    i264::QuantizeScale::Quantize(coefficients, qp, levels, false);
    std::cout << i264::FormatPrintableArray2D(levels) << std::endl;

    i264::Array2D<int32_t, 4, 4> recon_coefficients;
    i264::QuantizeScale::Dequantize(levels, qp, recon_coefficients, false);

    i264::Array2D<int32_t, 4, 4> recon_x;
    i264::Transform::Inverse4x4Residual(recon_coefficients, recon_x);
    std::cout << i264::FormatPrintableArray2D(recon_x) << std::endl;
  }
}
