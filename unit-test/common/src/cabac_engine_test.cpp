/**
 * @file cabac_engine_test.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-04-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cabac_engine_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/bac_engine.h"
#include "i264/common/bit_stream.h"
#include "i264/common/syntax_coder.h"
#include "i264/common/utils.h"

CabacEngineTest::CabacEngineTest() = default;

CabacEngineTest::~CabacEngineTest() = default;

void CabacEngineTest::SetUp() {}

void CabacEngineTest::TearDown() {}

TEST_F(CabacEngineTest, EndOfSlice) {
  i264::BitStreamWriter bs_writer;

  i264::BACCoder coder(&bs_writer);
  i264::BACState encode_state;

  std::vector<uint8_t> end_of_slice_flags(65536);

  for (int i = 0; i < 65535; i++) {
    end_of_slice_flags[i] = 0;
  }
  end_of_slice_flags[65535] = 1;

  for (int i = 0; i < 65536; i++) {
    coder.EncodeTerminate(end_of_slice_flags[i], encode_state);
  }

  size_t slice_length = bs_writer.GetNumberOfWrittenBits();
  std::cout << "SliceLength: " << slice_length << std::endl;

  i264::SyntaxCoder::CodeRbspSliceTrailingBits(bs_writer, true);
  size_t nal_length = bs_writer.GetNumberOfWrittenBits();
  std::cout << "NalLength: " << nal_length << std::endl;
  size_t nal_appended_length = nal_length - slice_length;

  // Decode
  i264::BitStreamReader bs_reader;
  i264::BACDecoder decoder(&bs_reader);
  i264::BACState decode_state;

  bs_reader.LoadBits(bs_writer.GetBitStream().Data(),
                     bs_writer.GetBitStream().Size());

  for (int i = 0; i < 65536; i++) {
    uint32_t bit_value = 0;
    if (i == 0)
      decoder.DecodeTerminate(decode_state, bit_value, true);
    else
      decoder.DecodeTerminate(decode_state, bit_value, false);

    ASSERT_EQ(bit_value, end_of_slice_flags[i]);
  }

  size_t bits_left = bs_reader.GetNumberOfRemainingBits();
  ASSERT_EQ(nal_appended_length, bits_left);
}
