/**
 * @file syntax_coder_test.cpp
 * @author Caoyang Jiang (www.hypevr.com)
 * @brief
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "syntax_coder_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/syntax_coder.h"

SyntaxCoderTest::SyntaxCoderTest() = default;

SyntaxCoderTest::~SyntaxCoderTest() = default;

void SyntaxCoderTest::SetUp() {}

void SyntaxCoderTest::TearDown() {}

TEST_F(SyntaxCoderTest, SimpleSPS) {
  i264::SPS sps;
  i264::BitStreamWriter writer;
  i264::SyntaxCoder::CodeSPS(sps, writer);

  auto bit_stream = writer.GetBitStream();
  std::cout << "Wrote: " << bit_stream.Size() << " bits." << std::endl;
}
