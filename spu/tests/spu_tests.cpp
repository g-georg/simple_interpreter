#include <cstddef>
#include <cstdint>

#include <gtest/gtest.h>

#include "core/spu.h"
#include "enums/opcode.h"
#include "enums/runtime_error.h"

using spu::Opcode;
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

void ExpectPopReturnAddress(Spu& spu, size_t expected_value) {
  size_t actual_value = 0;

  RuntimeError error = spu.PopReturnAddress(actual_value);

  EXPECT_EQ(error, RuntimeError::kOk);
  EXPECT_EQ(actual_value, expected_value);
}

void ExpectReturnStackEmpty(Spu& spu) {
  size_t value = 0;

  RuntimeError error = spu.PopReturnAddress(value);

  EXPECT_EQ(error, RuntimeError::kStackUnderflow);
}

void ExpectReadRegister(Spu& spu, size_t index, int32_t expected_value) {
  int32_t actual_value = 0;

  RuntimeError error = spu.ReadRegister(index, actual_value);

  EXPECT_EQ(error, RuntimeError::kOk);
  EXPECT_EQ(actual_value, expected_value);
}

void ExpectReadRam(Spu& spu, size_t address, int32_t expected_value) {
  int32_t actual_value = 0;

  RuntimeError error = spu.ReadRam(address, actual_value);

  EXPECT_EQ(error, RuntimeError::kOk);
  EXPECT_EQ(actual_value, expected_value);
}

}  // namespace

TEST(SpuTest, NewSpuHasEmptyStacks) {
  Spu spu;

  EXPECT_TRUE(spu.DataStack().Empty());
  EXPECT_TRUE(spu.ReturnStack().Empty());
}

TEST(SpuTest, PushDataAndPopDataWorkCorrectly) {
  Spu spu;

  spu.PushData(10);
  spu.PushData(20);

  ExpectPopData(spu, 20);
  ExpectPopData(spu, 10);
  ExpectDataStackEmpty(spu);
}

TEST(SpuTest, PushReturnAddressAndPopReturnAddressWorkCorrectly) {
  Spu spu;

  EXPECT_EQ(spu.PushReturnAddress(5), RuntimeError::kOk);
  EXPECT_EQ(spu.PushReturnAddress(10), RuntimeError::kOk);

  ExpectPopReturnAddress(spu, 10);
  ExpectPopReturnAddress(spu, 5);
  ExpectReturnStackEmpty(spu);
}

TEST(SpuTest, RegisterWriteAndReadWorkCorrectly) {
  Spu spu;

  RuntimeError error = spu.WriteRegister(0, 123);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectReadRegister(spu, 0, 123);
}

TEST(SpuTest, RamWriteAndReadWorkCorrectly) {
  Spu spu;

  RuntimeError error = spu.WriteRam(0, 321);

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectReadRam(spu, 0, 321);
}

TEST(SpuTest, ResetClearsSpuState) {
  Spu spu;

  spu.PushData(1);
  EXPECT_EQ(spu.PushReturnAddress(2), RuntimeError::kOk);
  EXPECT_EQ(spu.WriteRegister(0, 100), RuntimeError::kOk);
  EXPECT_EQ(spu.WriteRam(0, 200), RuntimeError::kOk);
  spu.SetInstructionPointer(10);

  spu.Reset();

  EXPECT_TRUE(spu.DataStack().Empty());
  EXPECT_TRUE(spu.ReturnStack().Empty());
  EXPECT_EQ(spu.GetInstructionPointer(), static_cast<size_t>(0));

  ExpectReadRegister(spu, 0, 0);
  ExpectReadRam(spu, 0, 0);
}

TEST(SpuTest, SetAndGetInstructionPointerWorkCorrectly) {
  Spu spu;

  spu.SetInstructionPointer(15);

  EXPECT_EQ(spu.GetInstructionPointer(), static_cast<size_t>(15));
}

TEST(SpuTest, AdvanceInstructionPointerWorksCorrectly) {
  Spu spu;

  spu.SetInstructionPointer(3);
  spu.AdvanceInstructionPointer(4);

  EXPECT_EQ(spu.GetInstructionPointer(), static_cast<size_t>(7));
}

TEST(SpuTest, SetBytecodeStoresProgram) {
  Spu spu;

  spu.SetBytecode({
      static_cast<int32_t>(Opcode::kPush),
      10,
      static_cast<int32_t>(Opcode::kHlt),
  });

  EXPECT_EQ(spu.GetBytecodeSize(), static_cast<size_t>(3));
  EXPECT_EQ(spu.GetBytecode()[0], static_cast<int32_t>(Opcode::kPush));
  EXPECT_EQ(spu.GetBytecode()[1], 10);
  EXPECT_EQ(spu.GetBytecode()[2], static_cast<int32_t>(Opcode::kHlt));
}

TEST(SpuTest, RunProgramWithPushAddAndHlt) {
  Spu spu;

  spu.SetBytecode({
      static_cast<int32_t>(Opcode::kPush),
      10,
      static_cast<int32_t>(Opcode::kPush),
      15,
      static_cast<int32_t>(Opcode::kAdd),
      static_cast<int32_t>(Opcode::kHlt),
  });

  RuntimeError error = spu.Run();

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 25);
}

TEST(SpuTest, RunProgramWithSeveralMathCommands) {
  Spu spu;

  spu.SetBytecode({
      static_cast<int32_t>(Opcode::kPush),
      10,
      static_cast<int32_t>(Opcode::kPush),
      5,
      static_cast<int32_t>(Opcode::kAdd),
      static_cast<int32_t>(Opcode::kPush),
      3,
      static_cast<int32_t>(Opcode::kMul),
      static_cast<int32_t>(Opcode::kHlt),
  });

  RuntimeError error = spu.Run();

  EXPECT_EQ(error, RuntimeError::kOk);
  ExpectPopData(spu, 45);
}

TEST(SpuTest, RunProgramStopsOnDivisionByZero) {
  Spu spu;

  spu.SetBytecode({
      static_cast<int32_t>(Opcode::kPush),
      10,
      static_cast<int32_t>(Opcode::kPush),
      0,
      static_cast<int32_t>(Opcode::kDiv),
      static_cast<int32_t>(Opcode::kHlt),
  });

  RuntimeError error = spu.Run();

  EXPECT_EQ(error, RuntimeError::kDivisionByZero);
}