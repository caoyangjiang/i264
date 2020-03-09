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

namespace i264 {
class enum ColorPlane {
  PLANE_Y = 0,
  PLANE_U = 1,
  PLANE_V = 2,
  PLANE_G = 0,
  PLANE_B = 1,
  PLANE_R = 2
};

template <class T, int D1, int D2>
using Matrix2D = std::array<sd::array<T, D1>, D2>;

}  // namespace i264
