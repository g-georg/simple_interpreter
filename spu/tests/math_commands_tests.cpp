#include <cstdint>

#include <gtest/gtest.h>

#include "commands/math_commands.h"
#include "core/spu.h"
#include "enums/runtime_error.h"

using spu::AddCommand;
using spu::DivCommand;
using spu::MulCommand;
using spu::RuntimeError;
using spu::Spu;
using spu::SqrtCommand;
using spu::SubCommand;

namespace {

void ExpectPopData(Spu& spu, int32_t expected_value) {
  int32_t actual_value = 0;

  RuntimeError error = spu.PopData(actual_value);

  EXPECT_EQ(error, RuntimeError::kOk);
  EXPECT_EQ(actual_value, expected_value);
}

}  // namespace

TEST(MathCommandsTest, AddCommandAddsTwoValues) {
  Spu spu;

  spu.PushData(7);
  spu.PushData(5);

  AddCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 12);
}

TEST(MathCommandsTest, SubCommandSubtractsSecondValueFromFirst) {
  Spu spu;

  spu.PushData(10);
  spu.PushData(3);

  SubCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 7);
}

TEST(MathCommandsTest, MulCommandMultipliesTwoValues) {
  Spu spu;

  spu.PushData(6);
  spu.PushData(4);

  MulCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 24);
}

TEST(MathCommandsTest, DivCommandDividesFirstValueBySecond) {
  Spu spu;

  spu.PushData(20);
  spu.PushData(4);

  DivCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 5);
}

TEST(MathCommandsTest, DivCommandReturnsErrorOnDivisionByZero) {
  Spu spu;

  spu.PushData(20);
  spu.PushData(0);

  DivCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kDivisionByZero);
}

TEST(MathCommandsTest, SqrtCommandCalculatesSquareRoot) {
  Spu spu;

  spu.PushData(25);

  SqrtCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 5);
}

TEST(MathCommandsTest, SqrtCommandReturnsErrorForNegativeValue) {
  Spu spu;

  spu.PushData(-25);

  SqrtCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kInvalidInput);
}

TEST(MathCommandsTest, AddCommandReturnsStackUnderflowIfOnlyOneValueExists) {
  Spu spu;

  spu.PushData(10);

  AddCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kStackUnderflow);
}

TEST(MathCommandsTest, MulCommandReturnsStackUnderflowIfStackIsEmpty) {
  Spu spu;

  MulCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kStackUnderflow);
}