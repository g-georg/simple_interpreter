#pragma once

#include <cstdint>

namespace assembler {

enum class ArgumentType : uint32_t {
  kNone          = 0,
  kNumber        = 1 << 0,
  kLabel         = 1 << 1,
  kRegister      = 1 << 2,
  kMemoryAddress = 1 << 3,
  kUnknown       = 1u << 31,
};

inline ArgumentType operator|(ArgumentType a, ArgumentType b) {
  return static_cast<ArgumentType>(
      static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool HasFlag(ArgumentType value, ArgumentType flag) {
  return (static_cast<uint32_t>(value) &
          static_cast<uint32_t>(flag)) != 0;
}

enum class AssemblyPass : std::uint8_t {
  kFirst,
  kFinal,
};

struct Argument {
  ArgumentType type  = ArgumentType::kUnknown;
  int value          = -1337;
};

}