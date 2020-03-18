/**
 * @file intra_16x16.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_ENCODER_INCLUDE_I264_ENCODER_INTRA_16X16_H_
#define MODULES_ENCODER_INCLUDE_I264_ENCODER_INTRA_16X16_H_

#include "i264/common/macroblock.h"
#include "i264/common/types.h"

namespace i264 {

class Intra16x16Predictor {
 public:
  /**
   * @brief Perform an intra 16x16 prediction on the luma component.
   *
   * @param macroblock Macroblock for which prediction signal is generated.
   * @param mode The type of the intra 16x16 prediction mode
   * @param result Prediction luma signal
   *
   * @return True if prediction succeed, false prediction can not be made.
   */
  static bool PredictLuma(Macroblock &macroblock, enum INTRA_16x16_MODE mode,
                          Array2D<uint8_t, 16, 16> &result);

  /**
   * @brief Perform an intra 16x16 prediction on the chroma component.
   *
   * @param macroblock Macroblock for which prediction signal is generated.
   * @param mode The type of the intra 16x16 prediction mode
   * @param result Predicted chroma signal
   *
   * @return True if prediction succeed, false prediction can be made.
   */
  static bool PredictChroma(Macroblock &macroblock, enum INTRA_16x16_MODE mode,
                            Array2D<uint8_t, 16, 16> &result);

 private:
  static bool VerticalPredict(Macroblock &macroblock,
                              Array2D<uint8_t, 16, 16> &result);
  static bool HorizontalPredict(Macroblock &macroblock,
                                Array2D<uint8_t, 16, 16> &result);
  static bool DCPredict(Macroblock &macroblock,
                        Array2D<uint8_t, 16, 16> &result);
  static bool PlanePredict(Macroblock &macroblock,
                           Array2D<uint8_t, 16, 16> &result);
};

}  // namespace i264

#endif  // MODULES_ENCODER_INCLUDE_I264_ENCODER_INTRA_16X16_H_
