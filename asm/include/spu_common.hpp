#pragma once

#include <cstddef>

namespace spu {

enum class CommandCode : int {
  kEmpty = 0,
  kPush  = 1,
  kPop   = 2,
  kAdd   = 3,
  kSub   = 4,
  kDiv   = 5,
  kMul   = 6,
  kSqrt  = 7,
  kOut   = 8,
  kIn    = 9,

  kJmp = 10,
  kJb  = 11,
  kJbe = 12,
  kJa  = 13,
  kJae = 14,
  kJe  = 15,
  kJne = 16,

  kCall = 17,
  kRet  = 18,

  kPushM = 19,
  kPopM  = 20,
  kDraw  = 21,

  kPushR = 35,
  kPopR  = 42,

  kHlt = -1
};

inline constexpr size_t kNumberOfRegisters   = 8;
inline constexpr size_t kRegisterNameLen     = 3;
inline constexpr size_t kShiftRegister       = 5;
inline constexpr size_t kMaxCommandValue     = static_cast<size_t>(CommandCode::kPopR);
inline constexpr size_t kMaxLabelLen         = 63;
inline constexpr size_t kMaxLabelsCount      = 10;
inline constexpr int    kAsmVersion          = 1338;

}