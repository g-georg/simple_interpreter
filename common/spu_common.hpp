#pragma once

#include <stdint.h>

namespace spu {

enum class Opcode : int32_t {
  kPush = 0,
  kPop,
  kAdd,
  kSub,
  kDiv,
  kMul,
  kSqrt,
  kOut,
  kIn,
  kJmp,
  kJb,
  kJbe,
  kJa,
  kJae,
  kJe,
  kJne,
  kCall,
  kRet,
  kPushm,
  kPopm,
  kDraw,
  kPushr,
  kPopr,
  kHlt,
  kCount
};

inline constexpr size_t kNumberOfRegisters = 8;

inline constexpr size_t kRegisterNameLen = 3;

inline constexpr size_t kMaxLabelLen = 63;

inline constexpr size_t kMaxLabelsCount = 10;

inline constexpr int kAsmVersion = 1338;

}