/**
 * @file types.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/bits.h"

namespace i264 {

/**
 * @brief All H.264 defined NAL unit types.
 *
 */
enum class NalUnitType {
  UNSPECIFIED = 0,
  CODED_SLICE_NON_IDR_PICTURE = 1,
  CODED_SLICE_DATA_PARTITION_A = 2,
  CODED_SLICE_DATA_PARTITION_B = 3,
  CODED_SLICE_DATA_PARTITION_C = 4,
  CODED_SLICE_IDR_PICTURE = 5,
  SUPPLEMENTAL_ENHANCEMENT_INFORMATION = 6,
  SEQUENCE_PARAMETER_SET = 7,
  PICTURE_PARAMETER_SET = 8,
  ACCESS_UNIT_DELIMITER = 9,
  END_OF_SEQUENCE = 10,
  END_OF_STREAM = 11,
  FILLER_DATA = 12,
  SEQUENCE_PARAMETER_SET_EXTENSION = 13,
  PREFIX_NAL_UNIT = 14,
  SUBSET_SEQUENCE_PARAMETER_SET = 15,
  DEPTH_PARAMETER_SET = 16,
  RESERVED_0 = 17,
  RESERVED_1 = 18,
  CODED_SLICE_AUXILIARY_CODED_PICTURE = 19,
  CODED_SLICE_EXTENSION = 20,
  CODED_SLICE_EXTENSION_DEPTH_3DAVC = 21,
  RESERVED_2 = 22,
  RESERVED_3 = 23
};

/**
 * @brief All supported color plane names.
 *
 */
enum class ColorPlane {
  PLANE_Y = 0,
  PLANE_U = 1,
  PLANE_V = 2,
  PLANE_G = 0,
  PLANE_B = 1,
  PLANE_R = 2
};

/**
 * @brief All possible SLICE types
 *
 */
enum class SliceType {
  B_SLICE = 0,
  P_SLICE = 1,
  I_SLICE = 2,
  NUMBER_OF_SLICE_TYPES = 3
};

enum class INTRA_BLOCK_TYPE { BLOCK_16x16 = 0, BLOCK_8x8 = 1, BLOCK_4x4 = 2 };

enum class FrameType { ORIGINAL = 0, RECON = 1, RESIDUAL = 2 };

/**
 * @brief All the 16x16 intra prediction modes
 *
 */
enum class INTRA_16x16_MODE { VERTICAL = 0, HORIZONTAL = 1, DC = 2, PLANE = 3 };

/**
 * @brief All the 4x4 intra prediction modes
 *
 */
enum class INTRA_4x4_MODE {
  VERTICAL = 0,
  HORIZONTAL = 1,
  DC = 2,
  PLANE = 3,
  DIAGONAL_DOWN_LEFT = 4,
  DIAGONAL_DOWN_RIGHT = 5,
  VERTICAL_RIGHT = 6,
  HORIZONTAL_DOWN = 7,
  VERTICAL_LEFT = 8,
  HORIZONTAL_UP = 9
};

enum class NEIGHBOR_MB_POS {
  TOP = 0;        // known as "A"
  TOP_RIGHT = 1;  // known as "B"
  LEFT = 2;       // known as "C"
  TOP_LEFT = 3;
};

/**
 * @brief Any two dimensional structure
 *
 */
template <class T, int D1, int D2>
using Array2D = std::array<sd::array<T, D1>, D2>;

template <class T, int D1>
using Array1D = std::array<T, D1>;

/**
 * @brief BitStream is more appropriate type name in H.264 context
 *
 */
using BitStream = Bit;

}  // namespace i264
