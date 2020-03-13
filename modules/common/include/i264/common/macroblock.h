/**
 * @file macroblock.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_

#include "i264/common/common.h"
#include "i264/common/global.h"

namespace i264 {
class Slice;

/**
 * @brief Macroblock should only hold macroblock related encoding/decoding data.
 *
 */
class Macroblock {
 public:
  Macroblock() = default;
  explicit Macroblock(const Slice* belonging_slice);

  // assign macroblock attributes and its associated buffers
  void AssignIndexWithinSlice(int index);
  void AssignLocationInPixel(int x, int y);
  void AssignReconBuffer(FrameBuffer* buffer_start_addr);
  void AssignSourceBuffer(FrameBuffer* buffer_start_addr);
  void AssignResidualBuffer(FrameBuffer* buffer_start_addr);
  void SetNeighborMacroblock(Macroblock* mb, enum NEIGHBOR_MB_POS position);
  const Macroblock* GetNeighborMacroblock(enum NEIGHBOR_MB_POS position) const;

  // retrieve pixels operations (designed for readability)
  template <class BLOCK_WIDTH, class BLOCK_HEIGHT>
  Array2D<uint8_t, BLOCK_WIDTH, BLOCK_WIDTH> GetOrignalPixels(
      int channel_id, enum FrameType type, int zscan_index) const;
  template <class SIZE>
  Array1D<uint8_t, SIZE> GetBottomMostRow4x4(int channel, enum FrameType type,
                                             int zscan_index) const;
  template <class SIZE>
  Array1D<uint8_t, SIZE> GetRightMostColumn4x4(int channel, enum FrameType type,
                                               int zscan_index) const;

  // after rate-distortion, set modes
  void StoreBestModeAsIntra16x16(enum INTRA_16x16_MODE modes);
  void StoreBestModeAsIntra4x4(int zscan_index, INTRA_4x4_MODE mode);
  void StoreBestModeAsIntra4x4(Array<enum INTRA_4x4_MODE, 16> modes);

  // go into residual buffer, residual = intra/inter compensate + dct + quantize
  void StoreLumaResidual(Array2D<16, 16> residual);
  void StoreLumaResidual(Array2D<4, 4> residual, int zscan_index);
  void StoreChromaResidual(Array2D<8, 8> residual);
  void StoreChromaResidual(Array2D<2, 2> residual, int zscan_index);

  // go into recon buffer. recon = residual + deblocking
  void StoreLumaRecon(Array2D<16, 16> residual);
  void StoreLumaRecon(Array2D<4, 4> residual, int zscan_index);
  void StoreChromaRecon(Array2D<8, 8> residual);
  void StoreChromaRecon(Array2D<2, 2> residual, int zscan_index);

 private:
  const Slice* slice_ = std::nullptr_t;
  int address_zscan_ = -1;
  Position<uint16_t> pos_2d_ = {0, 0};
  uint16_t type_ = 0;

  // coding
  int cbp_;
  std::array<CodedBitPattern, 3> cbp_struct_;
  // int intra16_mode_ = 0;
  uint8_t chroma_intra_prediction_mode_ = 0;

  // subblock access
  int block_x = 0;
  int block_y = 0;
  int block_y_aff = 0;
  int pix_x = 0;
  int pix_y = 0;
  int pix_c_x = 0;
  int pix_c_y = 0;
  int subblock_x = 0;
  int subblock_y = 0;

  // mode decision
  enum INTRA_BLOCK_TYPE best_block_type_;
  enum INTRA_16x16_MODE best_16x16_luma_mode_;
  Array2D<enum INTRA_4x4_MODE, 4, 4> best_4x4_mode_;

  // quantize related
  std::array<int, MAX_COLOR_PLANE> qp_;
  std::array<int, MAX_COLOR_PLANE> qp_scale_;
  bool is_lossless = false;
  bool is_intra_block = true;

  uint8_t intra_pred_modes[16];
  uint8_t intra_pred_modes8x8[16];

  // neighbor information
  Macroblock* top_mb_ = std::nullptr_t;
  Macroblock* top_right_mb_ = std::nullptr_t;
  Macroblock* top_left_mb_ = std::nullptr_t;
  Macroblock* left_mb_ = std::nullptr_t;
  Slice* belong_slice_ = std::nullptr_t;
};
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_
