/**
 * @file quantize.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/types.h"

namespace i264 {

// the pre-scaling matrix V is defined for QP0-5
// 1. for all other QP > 5, it is derived from this
//    V(QP) = dequant4_scale(QP%6) * 2^(QP/6)
// 2. v is roughly = Si(from idct normalization) * Qstep * 2^6
static const std::array<Array2D<uint8_t, 4, 4>, 6> dequant4_scale{
    // Defined in 8.5.9, V is expanded for QP 0-5
    // QP=0
    10, 13, 10, 13, 13, 16, 13, 16, 10, 13, 10, 13, 13, 16, 13, 16,
    // QP=1
    11, 14, 11, 14, 14, 18, 14, 18, 11, 14, 11, 14, 14, 18, 14, 18,
    // QP=2
    13, 16, 13, 16, 16, 20, 16, 20, 13, 16, 13, 16, 16, 20, 16, 20,
    // QP=3
    14, 18, 14, 18, 18, 23, 18, 23, 14, 18, 14, 18, 18, 23, 18, 23,
    // QP=4
    16, 20, 16, 20, 20, 25, 20, 25, 16, 20, 16, 20, 20, 25, 20, 25,
    // QP=5
    18, 23, 18, 23, 23, 29, 23, 29, 18, 23, 18, 23, 23, 29, 23, 29};

// The post-scaling matrix M is not defined in the standard
// but it is doing "inverse" of dequat4_scale.
// 1. for all other QP > 5, it is derived from this
//    M(QP) = quant4_scale(QP%6) / 2^(QP/6)
// 2. There is a relationship between quant4_scale and dequant4_scale
//    quant4_scale = Si(from idct normalization) * Sf(from dct normalization) *
//    2^21 / dequat4_scale;
//    (for detail read "The H.264 Advanced Video Compression Standard 2nd
//    Edition" Section 7.2.3.7)
// static const std::array<Array2D<uint16_t, 4, 4>, 6> quant4_scale{
//     {// QP=0
//      {13107, 8066, 13107, 8066},
//      {8066, 5243, 8066, 5243},
//      {13107, 8066, 13107, 8066},
//      {8066, 5243, 8066, 5243}},
//     {// QP=1
//      {11916, 7490, 11916, 7490},
//      {7490, 4660, 7490, 4660},
//      {11916, 7490, 11916, 7490},
//      {7490, 4660, 7490, 4660}},
//     {// QP=2
//      {10082, 6554, 10082, 6554},
//      {6554, 4194, 6554, 4194},
//      {10082, 6554, 10082, 6554},
//      {6554, 4194, 6554, 4194}},
//     {// QP=3
//      {9362, 5825, 9362, 5825},
//      {5825, 3647, 5825, 3647},
//      {9362, 5825, 9362, 5825},
//      {5825, 3647, 5825, 3647}},
//     {// QP=4
//      {8192, 5243, 8192, 5243},
//      {5243, 3355, 5243, 3355},
//      {8192, 5243, 8192, 5243},
//      {5243, 3355, 5243, 3355}},
//     {// QP=5
//      {7282, 4559, 7282, 4559},
//      {4559, 2893, 4559, 2893},
//      {7282, 4559, 7282, 4559},
//      {4559, 2893, 4559, 2893}}};

}  // namespace i264
