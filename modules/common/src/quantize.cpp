/**
 * @file quantize.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-23
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/quantize.h"

namespace i264 {
void QuantizeScale::QuantizeLumaDC(const Array2D<int32_t, 4, 4>& coefficients,
                                   const int qp,
                                   Array2D<int32_t, 4, 4>& levels) {
  // Defined in 8.5.10
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      const auto& coef = coefficients[i][j];
      auto& level = levels[i][j];
      if (coef >= 0) {
        level = ((coef * kQuant4_Scale[qp % 6][0][0]) + (1 << (15 + qp / 6))) >>
                (16 + qp / 6);
      } else {
        level =
            -(((-coef * kQuant4_Scale[qp % 6][0][0]) + (1 << (15 + qp / 6))) >>
              (16 + qp / 6));
      }
    }
  }
}

void QuantizeScale::DequantizeLumaDC(const Array2D<int32_t, 4, 4>& levels,
                                     const int qp,
                                     Array2D<int32_t, 4, 4>& coefficients) {
  // Defined in 8.5.10
  Array2D<int32_t, 4, 4> level_scale_4x4;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      level_scale_4x4[i][j] =
          static_cast<int32_t>(kDequant4_Scale[qp % 6][i][j]) << 4;
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      auto& coef = coefficients[i][j];
      auto& level = levels[i][j];
      if (qp >= 36) {
        if (level >= 0) {
          coef = (level * level_scale_4x4[0][0]) << (qp / 6 - 6);
        } else {
          coef = -((-level * level_scale_4x4[0][0]) << (qp / 6 - 6));
        }
      } else {
        if (level >= 0) {
          coef = ((level * level_scale_4x4[0][0]) + (1 << (5 - qp / 6))) >>
                 (6 - qp / 6);
        } else {
          coef = -(((-level * level_scale_4x4[0][0]) + (1 << (5 - qp / 6))) >>
                   (6 - qp / 6));
        }
      }
    }
  }
}

void QuantizeScale::QuantizeChromaDC(const Array2D<int32_t, 2, 2>& coefficients,
                                     const int qp,
                                     Array2D<int32_t, 2, 2>& levels) {
  // Defined in 8.5.10
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      const auto& coef = coefficients[i][j];
      auto& level = levels[i][j];
      if (coef >= 0) {
        level = ((coef * kQuant4_Scale[qp % 6][0][0]) + (1 << (15 + qp / 6))) >>
                (16 + qp / 6);
      } else {
        level =
            -(((-coef * kQuant4_Scale[qp % 6][0][0]) + (1 << (15 + qp / 6))) >>
              (16 + qp / 6));
      }
    }
  }
}

void QuantizeScale::DequantizeChromaDC(const Array2D<int32_t, 2, 2>& levels,
                                       const int qp,
                                       Array2D<int32_t, 2, 2>& coefficients) {
  // Defined in 8.5.11.2
  Array2D<int32_t, 2, 2> level_scale_4x4;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      level_scale_4x4[i][j] =
          static_cast<int32_t>(kDequant4_Scale[qp % 6][i][j]) << 4;
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      auto& coef = coefficients[i][j];
      auto& level = levels[i][j];

      if (level >= 0) {
        coef = ((level * level_scale_4x4[0][0]) << (qp / 6)) >> 5;
      } else {
        coef = -(((-level * level_scale_4x4[0][0]) << (qp / 6)) >> 5);
      }
    }
  }
}
}  // namespace i264
