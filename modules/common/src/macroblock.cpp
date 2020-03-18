/**
 * @file macroblock.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-12
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "i264/common/macroblock.h"

namespace i264 {

const Macroblock* Macroblock::GetNeighborMacroblock(
    enum NEIGHBOR_MB_POS position) const {
  switch (position) {
    case NEIGHBOR_MB_POS::LEFT:
      return left_mb_;  // known as MbAddrA
    case NEIGHBOR_MB_POS::TOP:
      return top_mb_;  // knwon as MbAddrB
    case NEIGHBOR_MB_POS::TOP_RIGHT:
      return top_right_mb_;  // knwon as MbAddrC
    case NEIGHBOR_MB_POS::TOP_LEFT:
      return top_left_mb_;  // knwon as MbAddrD
  }
}

FrameBuffer* Macroblock::GetFrameBufferFromType(enum FrameType type) {
  if (type == FrameType::ORIGINAL) return original_pixel_frame_buffer_;
  if (type == FrameType::RESIDUAL) return residual_frame_buffer_;
  if (type == FrameType::RECON) return reconstruct_frame_buffer_;
}

const FrameBuffer* Macroblock::GetFrameBufferFromType(
    enum FrameType type) const {
  if (type == FrameType::ORIGINAL) return original_pixel_frame_buffer_;
  if (type == FrameType::RESIDUAL) return residual_frame_buffer_;
  if (type == FrameType::RECON) return reconstruct_frame_buffer_;
}

}  // namespace i264
