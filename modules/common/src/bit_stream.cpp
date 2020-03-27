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

void BitStreamWriter::Write(uint32_t code, uint32_t length) {}

size_t BitStreamWriter::GetNumberOfBitsUntilByteAligned() const {
  return 8 - (bit_stream_.Size() % 8);
}

size_t BitStreamWriter::GetNumberOfWrittenBits() const {
  return bit_stream_.Size();
}

const BitStream& BitStreamWriter::GetBitStream() const { return bit_stream_; }

}  // namespace i264
