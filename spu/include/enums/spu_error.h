#pragma once

#include <stdint.h>

namespace spu {

enum class SpuError : uint32_t {
  kOk = 0,
  kNullStruct = 1u << 0,
  kFileOpen = 1u << 1,
  kParseError = 1u << 2,
  kWrongVersion = 1u << 3,
  kBytecodeOverflow = 1u << 4,
  kCommonError = 1u << 31
};

}  // namespace spu