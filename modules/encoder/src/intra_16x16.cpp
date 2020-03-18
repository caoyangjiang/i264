/**
 * @file intra_16x16.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/encoder/intra_16x16.h"

#include "i264/common/common.h"

namespace i264 {

bool Intra16x16Predictor::PredictLuma(
    Macroblock &macroblock, enum INTRA_16x16_MODE mode,
    Array2D<uint8_t, MB_SIZE, MB_SIZE> &result) {
  bool predict_succeed = false;

  if (mode == INTRA_16x16_MODE::VERTICAL) {
    predict_succeed = VerticalPredict(macroblock, result);
  }

  if (mode == INTRA_16x16_MODE::HORIZONTAL) {
    predict_succeed = HorizontalPredict(macroblock, result);
  }

  if (mode == INTRA_16x16_MODE::DC) {
    predict_succeed = DCPredict(macroblock, result);
  }

  if (mode == INTRA_16x16_MODE::PLANE) {
    predict_succeed = PlanePredict(macroblock, result);
  }

  return predict_succeed;
}  // namespace i264

bool Intra16x16Predictor::VerticalPredict(
    Macroblock &macroblock, Array2D<uint8_t, MB_SIZE, MB_SIZE> &result) {
  // Defined in 8.3.3.1
  auto top_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::TOP);
  bool is_predictable = top_mb != nullptr;

  if (is_predictable) {
    auto last_row = top_mb->GetBottomMostRow16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);

    for (int y = 0; y < MB_SIZE; y++) {
      for (int x = 0; x < MB_SIZE; x++) {
        result[y][x] = last_row[x];
      }
    }
  }

  return is_predictable;
}

bool Intra16x16Predictor::HorizontalPredict(
    Macroblock &macroblock, Array2D<uint8_t, MB_SIZE, MB_SIZE> &result) {
  // Defined in 8.3.3.2
  auto left_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::LEFT);
  bool is_predictable = left_mb != nullptr;

  if (is_predictable) {
    auto last_column = left_mb->GetRightMostColumn16x16<16>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);

    for (int y = 0; y < MB_SIZE; y++) {
      for (int x = 0; x < MB_SIZE; x++) {
        result[y][x] = last_column[y];
      }
    }
  }
  return is_predictable;
}

bool Intra16x16Predictor::DCPredict(
    Macroblock &macroblock, Array2D<uint8_t, MB_SIZE, MB_SIZE> &result) {
  // Defined in 8.3.3.3
  auto left_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::LEFT);
  auto top_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::TOP);

  uint32_t left_column_sum = 0, top_row_sum = 0;

  if (left_mb != nullptr) {
    auto last_column = left_mb->GetRightMostColumn16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);
    for (int i = 0; i < MB_SIZE; i++) {
      left_column_sum += last_column[i];
    }
  }

  if (top_mb != nullptr) {
    auto last_row = left_mb->GetBottomMostRow16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);
    for (int i = 0; i < MB_SIZE; i++) {
      top_row_sum += last_row[i];
    }
  }

  uint32_t predict_pixel = 0;
  if (left_mb != nullptr && top_mb != nullptr) {
    // both top and left exists
    predict_pixel = (left_column_sum + top_row_sum + 16) >> 5;
  } else if (left_mb == nullptr && top_mb == nullptr) {
    // both top and left nonexists
    predict_pixel = (1 << (BIT_DEPTH - 1));
  } else {
    // one of the two exists
    predict_pixel = (left_column_sum + top_row_sum + 8) >> 4;
  }

  predict_pixel = std::clamp(predict_pixel, PIXEL_MIN_VALUE, PIXEL_MAX_VALUE);
  for (int y = 0; y < MB_SIZE; y++) {
    for (int x = 0; x < MB_SIZE; x++) {
      result[y][x] = static_cast<uint8_t>(predict_pixel);
    }
  }

  return true;
}

bool Intra16x16Predictor::PlanePredict(
    Macroblock &macroblock, Array2D<uint8_t, MB_SIZE, MB_SIZE> &result) {
  // Defined in 8.3.3.4
  auto left_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::LEFT);
  auto top_mb = macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::TOP);
  auto top_left_mb =
      macroblock.GetNeighborMacroblock(NEIGHBOR_MB_POS::TOP_LEFT);

  bool is_predictable =
      (left_mb != nullptr) && (top_mb != nullptr) && (top_left_mb != nullptr);

  if (is_predictable) {
    auto top_left_row = top_left_mb->GetBottomMostRow16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);
    auto left_column = left_mb->GetRightMostColumn16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);
    auto top_row = top_mb->GetBottomMostRow16x16<MB_SIZE>(
        static_cast<int>(ColorPlane::PLANE_Y), FrameType::RESIDUAL);

    int32_t h = 0, v = 0;
    // form H and V
    for (int i = 0; i <= 6; i++) {
      h = (i + 1) * (static_cast<int32_t>(top_row[8 + i]) -
                     static_cast<int32_t>(top_row[8 - i]));

      v = (i + 1) * (static_cast<int32_t>(left_column[8 + i]) -
                     static_cast<int32_t>(left_column[8 - i]));
    }

    // take care when index is 7, it involves one pixel from top left row
    h = 8 * (static_cast<int32_t>(top_row[15]) -
             static_cast<int32_t>(top_left_row[15]));
    v = 8 * (static_cast<int32_t>(left_column[15]) -
             static_cast<int32_t>(top_left_row[15]));

    // form a, b, and c
    int32_t a = 16 * (left_column[15] + top_row[15]);
    int32_t b = (5 * h + 32) >> 6;
    int32_t c = (5 * v + 32) >> 6;

    for (int y = 0; y < MB_SIZE; y++) {
      for (int x = 0; x < MB_SIZE; x++) {
        int32_t predict_pixel = (a + b * (x - 7) + c * (y - 7) + 16) >> 5;
        result[y][x] = static_cast<uint8_t>(
            std::clamp(predict_pixel, static_cast<int32_t>(PIXEL_MIN_VALUE),
                       static_cast<int32_t>(PIXEL_MAX_VALUE)));
      }
    }
  }

  return is_predictable;
}

}  // namespace i264
