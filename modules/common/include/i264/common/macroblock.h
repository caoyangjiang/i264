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
#include "i264/common/frame.h"
#include "i264/common/global.h"
#include "i264/common/types.h"

namespace i264 {
class Slice;

/**
 * @brief Macroblock should only hold macroblock related encoding/decoding data.
 * Itself should never perform any actual encoding and decoding operations.
 * Macrob
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
  void AssignOriginalPixelBuffer(FrameBuffer* buffer_start_addr);
  void AssignResidualBuffer(FrameBuffer* buffer_start_addr);
  void SetNeighborMacroblock(const Macroblock* mb,
                             enum NEIGHBOR_MB_POS position);
  const Macroblock* GetNeighborMacroblock(enum NEIGHBOR_MB_POS position) const;

  // retrieve pixels operations (designed for readability)
  template <int BLOCK_WIDTH, int BLOCK_HEIGHT>
  Array2D<uint8_t, BLOCK_WIDTH, BLOCK_WIDTH> GetPixels(
      int channel_id, enum FrameType type, int raster_scan_index) const;

  template <int SIZE>
  Array1D<uint8_t, SIZE> GetBottomMostRow16x16(int channel_id,
                                               enum FrameType type) const;
  template <int SIZE>
  Array1D<uint8_t, SIZE> GetBottomMostRow4x4(int channel_id,
                                             enum FrameType type,
                                             int raster_scan_index) const;

  template <int SIZE>
  Array1D<uint8_t, SIZE> GetRightMostColumn16x16(int channel_id,
                                                 enum FrameType type) const;
  template <int SIZE>
  Array1D<uint8_t, SIZE> GetRightMostColumn4x4(int channel_id,
                                               enum FrameType type,
                                               int raster_scan_index) const;

  // after rate-distortion, set modes
  void StoreBestModeAsIntra16x16(enum INTRA_16x16_MODE modes);
  void StoreBestModeAsIntra4x4(int raster_scan_index, INTRA_4x4_MODE mode);
  void StoreBestModeAsIntra4x4(Array1D<enum INTRA_4x4_MODE, 16> modes);

  // go into residual buffer, residual = intra/inter compensate + dct + quantize
  void StoreLumaResidual(Array2D<int, 16, 16> residual);
  void StoreLumaResidual(Array2D<int, 4, 4> residual, int raster_scan_index);
  void StoreChromaResidual(Array2D<int, 8, 8> residual);
  void StoreChromaResidual(Array2D<int, 2, 2> residual, int raster_scan_index);

  // go into recon buffer. recon = residual + deblocking
  void StoreLumaRecon(Array2D<int, 16, 16> residual);
  void StoreLumaRecon(Array2D<int, 4, 4> residual, int raster_scan_index);
  void StoreChromaRecon(Array2D<int, 8, 8> residual);
  void StoreChromaRecon(Array2D<int, 2, 2> residual, int raster_scan_index);

 private:
  const Slice* slice_ = nullptr;

  int mb_x_ = 0;
  int mb_y_ = 0;
  int mb_id_within_slice_ = 0;

  int address_zscan_ = -1;
  uint16_t type_ = 0;

  // coding
  // int cbp_;
  // Array1D<CodedBitPattern, 3> cbp_struct_;
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
  const Macroblock* top_mb_ = nullptr;
  const Macroblock* top_right_mb_ = nullptr;
  const Macroblock* top_left_mb_ = nullptr;
  const Macroblock* left_mb_ = nullptr;
  const Slice* belong_slice_ = nullptr;

  FrameBuffer* original_pixel_frame_buffer_ = nullptr;
  FrameBuffer* reconstruct_frame_buffer_ = nullptr;
  FrameBuffer* residual_frame_buffer_ = nullptr;

 private:
  FrameBuffer* GetFrameBufferFromType(enum FrameType type);
  const FrameBuffer* GetFrameBufferFromType(enum FrameType type) const;
};

template <int SIZE>
Array1D<uint8_t, SIZE> Macroblock::GetBottomMostRow16x16(
    int channel_id, enum FrameType type) const {
  const FrameBuffer* frame_buffer = GetFrameBufferFromType(type);
  const uint8_t* mb_start_address = frame_buffer->Buffer(channel_id) + mb_x_ +
                                    mb_y_ * frame_buffer->Stride(channel_id);

  const uint8_t* last_row_address =
      mb_start_address + (SIZE - 1) * frame_buffer->Stride(channel_id);

  Array1D<uint8_t, SIZE> last_row;
  std::memcpy(last_row.data(), last_row_address, SIZE);
  return last_row;
}

template <int SIZE>
Array1D<uint8_t, SIZE> Macroblock::GetBottomMostRow4x4(
    int channel_id, enum FrameType type, int raster_scan_index) const {
  const FrameBuffer* frame_buffer = GetFrameBufferFromType(type);
  const uint8_t* mb_start_address = frame_buffer->Buffer(channel_id) + mb_x_ +
                                    mb_y_ * frame_buffer->Stride(channel_id);
  int horizontal_block_count = 16 / SIZE;
  int vertical_block_count = 16 / SIZE;
  int block_x = raster_scan_index / horizontal_block_count;
  int block_y = raster_scan_index / vertical_block_count;

  // top left of this SIZExSIZE in raster scan order
  const uint8_t* target_block_address =
      mb_start_address + block_x * SIZE +
      block_y * SIZE * frame_buffer->Stride(channel_id);
  const uint8_t* last_row_address =
      target_block_address + (SIZE - 1) * frame_buffer->Stride(channel_id);

  Array1D<uint8_t, SIZE> last_row;
  std::memcpy(last_row.data(), last_row_address, SIZE);
  return last_row;
}

template <int SIZE>
Array1D<uint8_t, SIZE> Macroblock::GetRightMostColumn16x16(
    int channel_id, enum FrameType type) const {
  const FrameBuffer* frame_buffer = GetFrameBufferFromType(type);
  const uint8_t* mb_start_address = frame_buffer->Buffer(channel_id) + mb_x_ +
                                    mb_y_ * frame_buffer->Stride(channel_id);

  const uint8_t* last_column_address = mb_start_address + SIZE - 1;

  Array1D<uint8_t, SIZE> last_column;

  for (int i = 0; i < SIZE; i++) {
    last_column[i] =
        *(last_column_address + i * frame_buffer->Stride(channel_id));
  }

  return last_column;
}

template <int SIZE>
Array1D<uint8_t, SIZE> Macroblock::GetRightMostColumn4x4(
    int channel_id, enum FrameType type, int raster_scan_index) const {
  const FrameBuffer* frame_buffer = GetFrameBufferFromType(type);
  const uint8_t* mb_start_address = frame_buffer->Buffer(channel_id) + mb_x_ +
                                    mb_y_ * frame_buffer->Stride(channel_id);
  int horizontal_block_count = 16 / SIZE;
  int vertical_block_count = 16 / SIZE;
  int block_x = raster_scan_index / horizontal_block_count;
  int block_y = raster_scan_index / vertical_block_count;

  // top left of this SIZExSIZE in raster scan order
  const uint8_t* target_block_address =
      mb_start_address + block_x * SIZE +
      block_y * SIZE * frame_buffer->Stride(channel_id);
  const uint8_t* last_column_address = target_block_address + (SIZE - 1);

  Array1D<uint8_t, SIZE> last_column;

  for (int i = 0; i < SIZE; i++) {
    last_column[i] =
        *(last_column_address + i * frame_buffer->Stride(channel_id));
  }

  return last_column;
}

}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_MACROBLOCK_H_
