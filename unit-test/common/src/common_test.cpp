// Copyright 2020 Caoyang Jiang

#include "common_test.h"

WINDOWS_DISABLE_ALL_WARNING
#include <string>
WINDOWS_ENABLE_ALL_WARNING

#include "i264/common/common.h"

CommonTest::CommonTest() = default;

CommonTest::~CommonTest() = default;

void CommonTest::SetUp()
{
}

void CommonTest::TearDown()
{
}

TEST_F(CommonTest, Simple)
{
  i264::Test test;

  ASSERT_EQ(test.Run(), 0);
}
