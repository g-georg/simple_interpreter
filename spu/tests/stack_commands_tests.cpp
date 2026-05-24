#include <cstdint>

#include <gtest/gtest.h>

#include "commands/stack_commands.h"
#include "core/spu.h"
#include "enums/opcode.h"
#include "enums/runtime_error.h"

using spu::Opcode;
using spu::PopCommand;
using spu::PushCommand;
using spu::RuntimeError;
using spu::Spu;

namespace {

void ExpectPopData(Spu& spu, int32_t expected_value) {
  int32_t actual_value = 0;

  RuntimeError error = spu.PopData(actual_value);

  EXPECT_EQ(error, RuntimeError::kOk);
  EXPECT_EQ(actual_value, expected_value);
}

void ExpectDataStackEmpty(Spu& spu) {
  int32_t value = 0;

  RuntimeError error = spu.PopData(value);

  EXPECT_EQ(error, RuntimeError::kStackUnderflow);
}

}  // namespace

TEST(StackCommandsTest, PushCommandPushesImmediateValue) {
  Spu spu;

  spu.SetBytecode({
      static_cast<int32_t>(Opcode::kPush),
      123,
  });

  spu.SetInstructionPointer(1);

  PushCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 123);
}

TEST(StackCommandsTest, PopCommandRemovesTopValue) {
  Spu spu;

  spu.PushData(777);

  PopCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectDataStackEmpty(spu);
}

TEST(StackCommandsTest, PopCommandReturnsStackUnderflowForEmptyStack) {
  Spu spu;

  PopCommand command;
  RuntimeError error = command.Execute(spu);

  EXPECT_EQ(error, RuntimeError::kStackUnderflow);
}