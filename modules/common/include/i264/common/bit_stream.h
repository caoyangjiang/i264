/**
 * @file bit_stream.h
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MODULES_COMMON_INCLUDE_I264_COMMON_BIT_STREAM_H_
#define MODULES_COMMON_INCLUDE_I264_COMMON_BIT_STREAM_H_

#include "i264/common/types.h"

namespace i264 {
class BitStreamWriter {
 public:
  void WriteOne();
  void WriteZero();
  void WriteAlignOne();
  void WriteAlignZero();

  template <class TCODE, class TLEN,
            std::enable_if_t<std::is_unsigned<TCODE>::value, void>>
  inline void Write(TCODE code, TLEN length) {
    while (length > 0) {
      bit_stream_.Push(
          static_cast<BitStream::value_type>((code >> (length - 1)) & 0x01));
      length--;
    }
  }

  size_t GetNumberOfBitsUntilByteAligned() const;
  size_t GetNumberOfWrittenBits() const;
  const BitStream& GetBitStream() const;

 private:
  BitStream bit_stream_;
};
}  // namespace i264

#endif  // MODULES_COMMON_INCLUDE_I264_COMMON_BIT_STREAM_H_
