#include <cstddef>
#include <cstdint>

#include <gtest/gtest.h>

#include "containers/stack.h"

using spu::Stack;

TEST(StackTest, NewStackIsEmpty) {
  Stack<int32_t> stack;

  EXPECT_TRUE(stack.Empty());
  EXPECT_EQ(stack.Size(), static_cast<size_t>(0));
}

TEST(StackTest, PushAddsElements) {
  Stack<int32_t> stack;

  stack.Push(10);
  stack.Push(20);
  stack.Push(30);

  EXPECT_FALSE(stack.Empty());
  EXPECT_EQ(stack.Size(), static_cast<size_t>(3));
  EXPECT_EQ(stack.Top(), 30);
}

TEST(StackTest, PopRemovesElementsInReverseOrder) {
  Stack<int32_t> stack;

  stack.Push(10);
  stack.Push(20);
  stack.Push(30);

  EXPECT_EQ(stack.Pop(), 30);
  EXPECT_EQ(stack.Pop(), 20);
  EXPECT_EQ(stack.Pop(), 10);

  EXPECT_TRUE(stack.Empty());
  EXPECT_EQ(stack.Size(), static_cast<size_t>(0));
}

TEST(StackTest, TopDoesNotRemoveElement) {
  Stack<int32_t> stack;

  stack.Push(42);

  EXPECT_EQ(stack.Top(), 42);
  EXPECT_EQ(stack.Size(), static_cast<size_t>(1));
  EXPECT_EQ(stack.Top(), 42);
}

TEST(StackTest, CanStoreSeveralDifferentValues) {
  Stack<int32_t> stack;

  stack.Push(-5);
  stack.Push(0);
  stack.Push(100);

  EXPECT_EQ(stack.Pop(), 100);
  EXPECT_EQ(stack.Pop(), 0);
  EXPECT_EQ(stack.Pop(), -5);
}