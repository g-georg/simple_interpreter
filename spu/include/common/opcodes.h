#pragma once

#include <cstdint>

namespace spu {

enum class Opcode : std::int32_t {
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

}  // namespace spu
