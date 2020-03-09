/**
 * @file transform.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/types.h"

namespace i264 {

class CosineTransform {
 public:
  static void Forward4x4(const int *data_in, int in_stride,
                         Matrix2D<int, 4, 4> *data_out);
  static void Inverse4x4(const Matrix2D<int, 4, 4> data_in, int out_stride,
                         int *data_in);

  static void Forward8x8(const int *data_in, int in_stride,
                         Matrix2D<int, 8, 8> *data_out);
  static void Inverse8x8(const Matrix2D<int, 8, 8> data_in, int out_stride,
                         int *data_in);
};

class HadamardTransform {
 public:
  static void Forward4x4(const int *data_in, int in_stride,
                         Matrix2D<int, 4, 4> *data_out);
  static void Inverse4x4(const Matrix2D<int, 4, 4> data_in, int out_stride,
                         int *data_in);

  static void Forward4x2(const int *data_in, int in_stride,
                         Matrix2D<int, 4, 4> *data_out);
  static void Inverse4x2(const Matrix2D<int, 4, 4> data_in, int out_stride,
                         int *data_in);

  static void Forward2x2(const int *data_in, int in_stride,
                         Matrix2D<int, 4, 4> *data_out);
  static void Inverse2x2(const Matrix2D<int, 4, 4> data_in, int out_stride,
                         int *data_in);
};

}  // namespace i264
