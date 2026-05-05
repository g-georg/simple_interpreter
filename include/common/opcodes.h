#ifndef INCLUDE_COMMON_OPCODES_H_
#define INCLUDE_COMMON_OPCODES_H_

#include <cstdint>

namespace spu {

enum class Opcode : int32_t {
  kEmpty = 0,
  kPush = 1,
  kPop = 2,
  kAdd = 3,
  kSub = 4,
  kDiv = 5,
  kMul = 6,
  kSqrt = 7,
  kOut = 8,
  kIn = 9,
  kJmp = 10,
  kJb = 11,
  kJbe = 12,
  kJa = 13,
  kJae = 14,
  kJe = 15,
  kJne = 16,
  kCall = 17,
  kRet = 18,
  kPushm = 19,
  kPopm = 20,
  kDraw = 21,
  kPushr = 35,
  kPopr = 42,
  kHlt = -1,
};

}

#endif