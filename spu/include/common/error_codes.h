#ifndef INCLUDE_COMMON_ERROR_CODES_H_
#define INCLUDE_COMMON_ERROR_CODES_H_

#include <cstdint>

namespace spu {

enum class SpuError : uint32_t {
  kOk = 0,
  kNullStruct = 1u << 0,
  kBytecodeNull = 1u << 1,
  kBytecodeOverflow = 1u << 2,
  kWrongVersion = 1u << 3,
  kCommonError = 1u << 31
};

enum class RuntimeError : uint32_t {
  kOk = 0,
  kMissingArgument = 1u << 0,
  kNotEnoughElementsOnStack = 1u << 1,
  kDivisionByZero = 1u << 2,
  kSqrtNegativeArgument = 1u << 3,
  kInvalidInput = 1u << 4,
  kUnknownBytecode = 1u << 5,
  kJmpArgumentIsNegative = 1u << 6,
  kCallArgumentIsNegative = 1u << 7,
  kRetValueIsNegative = 1u << 8
};

}

#endif