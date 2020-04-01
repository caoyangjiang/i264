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

CabacEngineTest::CabacEngineTest() = default;

CabacEngineTest::~CabacEngineTest() = default;

void CabacEngineTest::SetUp() {}

void CabacEngineTest::TearDown() {}

TEST_F(CabacEngineTest, Code) {
  i264::BitStreamWriter bs_writer;

  i264::BACCoder coder(&bs_writer);
  i264::BACState state;
  i264::BACProbabilityModel model;

  model.probability_state_index = 63;
  //   for (uint16_t i = 0; i < 1; i++) {
  coder.EncodeTerminate(1u, state);
  //   }

  std::cout << "Final pStateIndex: "
            << static_cast<uint32_t>(model.probability_state_index)
            << std::endl;
  std::cout << "Final MPS: "
            << static_cast<uint32_t>(model.most_probable_symbol) << std::endl;
  std::cout << "CodeLength: " << bs_writer.GetNumberOfWrittenBits()
            << std::endl;

  auto& bit_stream = bs_writer.GetBitStream();
  for (size_t i = 0; i < bit_stream.Size(); i++) {
    std::cout << static_cast<uint32_t>(bit_stream[i]) << " ";
  }
  std::cout << std::endl;
}
