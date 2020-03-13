/**
 * @file mb_encoder.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_ENCODER_INCLUDE_I264_ENCODER_MACROBLOCK_ENCODER_H_
#define MODULES_ENCODER_INCLUDE_I264_ENCODER_MACROBLOCK_ENCODER_H_

#include "i264/common/global.h"
#include "i264/common/macroblock.h"
#include "i264/encoder/slice_encoder.h"

namespace i264 {

class MacroblockEncoder {
 public:
  MacroblockEncoder(const SliceEncoder& slice_encoder);

  /**
   * @brief Encode a given macroblock.
   *
   * Pre-condition:
   * - macroblock address within the slice must be set.
   * - macroblock belonging slice must be set.
   * - macroblock block type is determined.
   *
   * Post-condition:
   * - encoding mode and residual signal are calculated.
   * - macroblock_layer() are encoded.
   *
   * @param macroblock
   */
  void Encode(Macroblock& macroblock);

  // void SetIntraCostFunction();
 private:
  void TryIntra16x16(Macroblock& macroblock, int& cost,
                     enum INTRA_16x16_MODE& best_mode);
  void TryIntra4x4(Macroblock& macroblock, int& cost,
                   Array1D<enum INTRA_4x4_MODE, 16>& best_mode);

  // Array2D<16, 16> GenerateIntra16x16PredictSignal(int modes, int channel)
  // const; Array2D<8, 8> GenerateIntra8x8PredictSignal(int zscan_index, int
  // modes,
  //                                             int channel) const;
  // Array2D<4, 4> GenerateIntra4x4PredictSignal(int zscan_index, int modes,
  //                                             int channel) const;
  // Array2D<2, 2> GenerateIntra2x2PredictSignal(int zscan_index, int modes,
  //                                             int channel) const;

 private:
  const SliceEncoder& slice_encoder_;
};
}  // namespace i264

#endif  // MODULES_ENCODER_INCLUDE_I264_ENCODER_MACROBLOCK_ENCODER_H_
