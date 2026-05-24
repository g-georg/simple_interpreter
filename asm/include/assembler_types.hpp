#pragma once

#include <cstdint>
#include <concepts>
#include <type_traits>

namespace assembler {

template<typename T>
concept BitmaskEnum = std::is_enum_v<T> &&
                      std::is_same_v<std::underlying_type_t<T>, uint32_t>;

template<BitmaskEnum T>
inline T operator|(T a, T b) {
  return static_cast<T>(
      static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

template<BitmaskEnum T>
inline bool HasFlag(T value, T flag) {
  return (static_cast<uint32_t>(value) &
          static_cast<uint32_t>(flag)) != 0;
}

enum class ArgumentType : uint32_t {
  kNone          = 0,
  kNumber        = 1 << 0,
  kLabel         = 1 << 1,
  kRegister      = 1 << 2,
  kMemoryAddress = 1 << 3,
  kUnknown       = 1u << 31,
};

enum class AssemblyPass : uint8_t {
  kFirst,
  kFinal,
};

struct Argument {
  ArgumentType type  = ArgumentType::kUnknown;
  int          value = 0;
};

}