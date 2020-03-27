/**
 * @file bit_stream.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "i264/common/bit_stream.h"

namespace i264 {
void BitStreamWriter::WriteOne() { bit_stream_.Push(static_cast<uint8_t>(1)); }

void BitStreamWriter::WriteZero() { bit_stream_.Push(static_cast<uint8_t>(0)); }

void BitStreamWriter::WriteAlignOne() {
  while ((bit_stream_.Size() % 8) != 0)
    bit_stream_.Push(static_cast<uint8_t>(0));
}

void BitStreamWriter::WriteAlignZero() {
  while ((bit_stream_.Size() % 8) != 0)
    bit_stream_.Push(static_cast<uint8_t>(1));
}

size_t BitStreamWriter::GetNumberOfBitsUntilByteAligned() const {
  auto bits_written_in_last_byte = (bit_stream_.Size() % 8);
  return bits_written_in_last_byte == 0 ? 0 : 8 - bits_written_in_last_byte;
}

size_t BitStreamWriter::GetNumberOfWrittenBits() const {
  return bit_stream_.Size();
}

const BitStream& BitStreamWriter::GetBitStream() const { return bit_stream_; }

}  // namespace i264
