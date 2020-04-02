/**
 * @file bac_engine.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-31
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/bac_engine.h"

#include "i264/common/bit_stream.h"
#include "i264/common/types.h"

namespace i264 {

/**
 * @brief  Defined in Table 9-44 - Specification of range TableLPS depending on
 * pStateIdx and qCodIRangeIdx
 *
 */
static const Array2D<uint8_t, 64, 4> kCabacRangeTableLPS = {
    128, 176, 208, 240,  //
    128, 167, 197, 227,  //
    128, 158, 187, 216,  //
    123, 150, 178, 205,  //
    116, 142, 169, 195,  //
    111, 135, 160, 185,  //
    105, 128, 152, 175,  //
    100, 122, 144, 166,  //
    95,  116, 137, 158,  //
    90,  110, 130, 150,  //
    85,  104, 123, 142,  //
    81,  99,  117, 135,  //
    77,  94,  111, 128,  //
    73,  89,  105, 122,  //
    69,  85,  100, 116,  //
    66,  80,  95,  110,  //
    62,  76,  90,  104,  //
    59,  72,  86,  99,   //
    56,  69,  81,  94,   //
    53,  65,  77,  89,   //
    51,  62,  73,  85,   //
    48,  59,  69,  80,   //
    46,  56,  66,  76,   //
    43,  53,  63,  72,   //
    41,  50,  59,  69,   //
    39,  48,  56,  65,   //
    37,  45,  54,  62,   //
    35,  43,  51,  59,   //
    33,  41,  48,  56,   //
    32,  39,  46,  53,   //
    30,  37,  43,  50,   //
    29,  35,  41,  48,   //
    27,  33,  39,  45,   //
    26,  31,  37,  43,   //
    24,  30,  35,  41,   //
    23,  28,  33,  39,   //
    22,  27,  32,  37,   //
    21,  26,  30,  35,   //
    20,  24,  29,  33,   //
    19,  23,  27,  31,   //
    18,  22,  26,  30,   //
    17,  21,  25,  28,   //
    16,  20,  23,  27,   //
    15,  19,  22,  25,   //
    14,  18,  21,  24,   //
    14,  17,  20,  23,   //
    13,  16,  19,  22,   //
    12,  15,  18,  21,   //
    12,  14,  17,  20,   //
    11,  14,  16,  19,   //
    11,  13,  15,  18,   //
    10,  12,  15,  17,   //
    10,  12,  14,  16,   //
    9,   11,  13,  15,   //
    9,   11,  12,  14,   //
    8,   10,  12,  14,   //
    8,   9,   11,  13,   //
    7,   9,   11,  12,   //
    7,   9,   10,  12,   //
    7,   8,   10,  11,   //
    6,   8,   9,   11,   //
    6,   7,   9,   10,   //
    6,   7,   8,   9,    //
    2,   2,   2,   2     //
};

/**
 * @brief Defined in Table 9-45 - State transition table
 *
 */
static const Array2D<uint8_t, 64, 2> kCabacTransitionTable = {
    0,  1,  0,  2,  1,  3,  2,  4,  2,  5,  4,  6,  4,  7,  5,  8,  6,  9,  7,
    10, 8,  11, 9,  12, 9,  13, 11, 14, 11, 15, 12, 16, 13, 17, 13, 18, 15, 19,
    15, 20, 16, 21, 16, 22, 18, 23, 18, 24, 19, 25, 19, 26, 21, 27, 21, 28, 22,
    29, 22, 30, 23, 31, 24, 32, 24, 33, 25, 34, 26, 35, 26, 36, 27, 37, 27, 38,
    28, 39, 29, 40, 29, 41, 30, 42, 30, 43, 30, 44, 31, 45, 32, 46, 32, 47, 33,
    48, 33, 49, 33, 50, 34, 51, 34, 52, 35, 53, 35, 54, 35, 55, 36, 56, 36, 57,
    36, 58, 37, 59, 37, 60, 37, 61, 38, 62, 38, 62, 63, 63};

BACProbabilityModel::BACProbabilityModel(int32_t m, int32_t n,
                                         int32_t slice_qp_y) {
  Reset(m, n, slice_qp_y);
}

void BACProbabilityModel::Update(uint32_t bin_value) {
  // Figure 8-7 - Flowchart for encoding a decision
  if (bin_value != most_probable_symbol) {  // Bin is LPS
    if (probability_state_index == 0) {
      most_probable_symbol = 1 - most_probable_symbol;
    } else {
      probability_state_index =
          kCabacTransitionTable[probability_state_index][0];
    }
  } else {  // Bin is MPS
    probability_state_index = kCabacTransitionTable[probability_state_index][1];
  }
}

void BACProbabilityModel::Reset() {
  // see G.9.3.1 Initialisation process
  int32_t pre_ctx_state = 0;

  if (m > 0) {
    std::clamp(1, 126, ((m * std::clamp(0, 51, slice_qp_y)) >> 4) + n);
  } else {
    std::clamp(1, 126, -((-m * std::clamp(0, 51, slice_qp_y)) >> 4) + n);
  }

  if (pre_ctx_state <= 63) {
    probability_state_index = 63 - pre_ctx_state;
    most_probable_symbol = 0;
  } else {
    probability_state_index = pre_ctx_state - 64;
    most_probable_symbol = 1;
  }
}

void BACProbabilityModel::Reset(int32_t m, int32_t n, int32_t slice_qp_y) {
  // see G.9.3.1 Initialisation process
  this->m = m;
  this->n = n;
  this->slice_qp_y = slice_qp_y;
  Reset();
}

BACCoder::BACCoder(BitStreamWriter* writer) : writer_(writer) {}

void BACCoder::SetBitStreamWriter(BitStreamWriter* writer) { writer_ = writer; }

void BACCoder::Encode(uint32_t bin_value, BACState& state,
                      BACProbabilityModel& model) {
  assert(writer_ != nullptr);

  // Figure 9-7 - Flowchart for encoding a decision
  uint16_t q_code_i_range_index = (state.range >> 6) & 3;
  uint16_t code_i_range_lps =
      kCabacRangeTableLPS[model.probability_state_index][q_code_i_range_index];
  state.range = state.range - code_i_range_lps;

  if (bin_value != model.most_probable_symbol) {
    state.low = state.low + state.range;
    state.range = code_i_range_lps;
  }

  // middle boxes
  model.Update(bin_value);

  // Figure 9-9 - Flowchart of PutBit(B)
  auto put_bit_lambda = [&](uint16_t bit) {
    if (state.first_bit_flag != 0) {
      state.first_bit_flag = 0;
    } else {
      writer_->Write(bit, 1);
    }

    while (state.bit_outstanding > 0) {
      writer_->Write(1ul - bit, 1);
      state.bit_outstanding--;
    }
  };

  // RenormE box
  for (;;) {
    if (state.range < 256) {
      if (state.low < 256) {
        put_bit_lambda(0);
      } else {
        if (state.low >= 512) {
          state.low = state.low - 512;
          put_bit_lambda(1);
        } else {
          state.low = state.low - 256;
          state.bit_outstanding++;
        }
      }

      state.range <<= 1;
      state.low <<= 1;
    } else {
      break;
    }
  }

  // last box
  bin_counts_in_nal_units_++;
}

void BACCoder::EncodeTerminate(uint32_t bin_value, BACState& state) {
  // PutBit box
  auto put_bit_lambda = [&](uint16_t bit) {
    if (state.first_bit_flag != 0) {
      state.first_bit_flag = 0;
    } else {
      writer_->Write(bit, 1);
    }

    while (state.bit_outstanding > 0) {
      writer_->Write(1ul - bit, 1);
      state.bit_outstanding--;
    }
  };

  // RenormE box
  auto renorm_e_lambda = [&]() {
    for (;;) {
      if (state.range < 256) {
        if (state.low < 256) {
          put_bit_lambda(0);
        } else {
          if (state.low >= 512) {
            state.low = state.low - 512;
            put_bit_lambda(1);
          } else {
            state.low = state.low - 256;
            state.bit_outstanding++;
          }
        }

        state.range <<= 1;
        state.low <<= 1;
      } else {
        break;
      }
    }
  };

  // Figure 9-12 - Flowchart of flushing at termination
  auto encode_flush_lambda = [&]() {
    state.range = 2;
    std::invoke(renorm_e_lambda);
    std::invoke(put_bit_lambda, (state.low >> 9) & 0x01);
    writer_->Write((((state.low >> 7) & 0x03u)) | 1u, 2);
  };

  // Figure 9-11 - Flowchart of encoding a decision before termination
  state.range = state.range - 2;
  if (bin_value != 0) {
    state.low = state.low + state.range;
    std::invoke(encode_flush_lambda);
  } else {
    std::invoke(renorm_e_lambda);
  }

  bin_counts_in_nal_units_++;
}

void BACCoder::Reset() { bin_counts_in_nal_units_ = 0; }

BACDecoder::BACDecoder(BitStreamReader* reader) : reader_(reader) {}

void BACDecoder::SetBitStreamReader(BitStreamReader* reader) {
  reader_ = reader;
}

void BACDecoder::Decode(BACState& state, BACProbabilityModel& model,
                        uint32_t& bin_value) {}

void BACDecoder::DecodeTerminate(BACState& state, uint32_t& bin_value,
                                 bool init_state) {
  if (init_state) {
    // Defined in 9.3.1.2 Initialisation process for the arithmetic decoding
    // engine.
    reader_->Read(9, state.offset);
  }

  auto renorm_d = [&]() {
    for (;;) {
      if (state.range < 256) {
        state.range <<= 1;
        state.offset <<= 1;
        uint16_t bit = 0;
        reader_->Read(1, bit);
        state.offset |= bit;
      } else {
        break;
      }
    }
  };

  state.range = state.range - 2;

  if (state.offset >= state.range) {
    bin_value = 1;
  } else {
    bin_value = 0;
    std::invoke(renorm_d);
  }
}

void BACDecoder::Reset() { bin_counts_in_nal_units_ = 0; }

}  // namespace i264
