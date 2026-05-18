#pragma once

#include <stdint.h>

namespace spu {

enum class RuntimeError : uint32_t {
  kOk = 0,
  kNullStruct = 1u << 0,
  kMissingArgument = 1u << 1,
  kStackUnderflow = 1u << 2,
  kDivisionByZero = 1u << 3,
  kSqrtNegativeArgument = 1u << 4,
  kInvalidInput = 1u << 5,
  kUnknownBytecode = 1u << 6,
  kJmpArgumentIsNegative = 1u << 7,
  kJmpArgumentOutOfRange = 1u << 8,
  kCallArgumentIsNegative = 1u << 9,
  kCallArgumentOutOfRange = 1u << 10,
  kRetValueIsNegative = 1u << 11,
  kInvalidRegister = 1u << 12,
  kInvalidRamAddress = 1u << 13
};

}  // namespace spu