/**
 * @file bac_engine.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_BAC_ENGINE_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_BAC_ENGINE_H_

#include <cstdint>

namespace i264 {
class BitStreamWriter;
class BitStreamReader;

struct BACState {
  // for encode & decode
  uint16_t range = 510;
  uint16_t low = 0;
  uint32_t bit_outstanding = 0;
  bool first_bit_flag = 1;

  // for decode only
  uint16_t offset = 0;

  inline void Reset() {
    range = 510;
    low = 0;
    bit_outstanding = 0;
    first_bit_flag = 1;
    offset = 0;
  }
};

struct BACProbabilityModel {
  uint8_t most_probable_symbol = 0;     // valMPS
  uint8_t probability_state_index = 0;  // pStateIdx

  // variable for initialize this model
  // see G.9.3.1 Initialisation process
  int32_t m = 0;
  int32_t n = 0;
  int32_t slice_qp_y = 0;

  BACProbabilityModel() = default;
  BACProbabilityModel(int32_t m, int32_t n, int32_t slice_qp_y);
  void Update(uint32_t bin_value);
  void Reset();
  void Reset(int32_t m, int32_t n, int32_t slice_qp_y);
};

/**
 * @brief Given a probability state, coding state, and current symbol value,
 * binary arithmetic coding (BAC) coder will encode the symbol alue, update
 * coding state and probability model.
 *
 * BAC coder DO NOT care about the syntax value, binarization, context, or
 * probability model initialization and reset.
 *
 */
class BACCoder {
 public:
  BACCoder() = default;
  explicit BACCoder(BitStreamWriter* writer);
  void SetBitStreamWriter(BitStreamWriter* writer);
  void Encode(uint32_t bin_value, BACState& state, BACProbabilityModel& model);
  void EncodeTerminate(uint32_t bin_value, BACState& state);
  void Reset();

 private:
  BitStreamWriter* writer_ = nullptr;
  uint64_t bin_counts_in_nal_units_ = 0;
};

/**
 * @brief Given a probability state and coding, binary arithmetic coding (BAC)
 * decoder will decode a bin_value from the bit stream.
 *
 */
class BACDecoder {
 public:
  BACDecoder() = default;
  explicit BACDecoder(BitStreamReader* reader);
  void SetBitStreamReader(BitStreamReader* reader);
  void Decode(BACState& state, BACProbabilityModel& model, uint32_t& bin_value);
  void DecodeTerminate(BACState& state, uint32_t& bin_value, bool initialize);
  void Reset();

 private:
  BitStreamReader* reader_ = nullptr;
  uint64_t bin_counts_in_nal_units_ = 0;
};
}  // namespace i264

#endif  //  MODULES_COMMON_INCLUDE_I264_COMMON_BAC_ENGINE_H_
