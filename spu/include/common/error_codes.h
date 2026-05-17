#pragma once

#include <cstdint>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace spu {

enum class SpuError : std::uint32_t {
  kOk = 0,
  kNullStruct = 1u << 0,
  kFileOpen = 1u << 1,
  kParseError = 1u << 2,
  kWrongVersion = 1u << 3,
  kBytecodeOverflow = 1u << 4,
  kCommonError = 1u << 31
};

enum class RuntimeError : std::uint32_t {
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

constexpr SpuError operator|(SpuError lhs, SpuError rhs) noexcept {
  return static_cast<SpuError>(ToUnderlying(lhs) | ToUnderlying(rhs));
}

constexpr RuntimeError operator|(RuntimeError lhs, RuntimeError rhs) noexcept {
  return static_cast<RuntimeError>(ToUnderlying(lhs) | ToUnderlying(rhs));
}

constexpr RuntimeError& operator|=(RuntimeError& lhs, RuntimeError rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

constexpr bool HasFlag(RuntimeError value, RuntimeError flag) noexcept {
  return (ToUnderlying(value) & ToUnderlying(flag)) != 0;
}

constexpr bool HasFlag(SpuError value, SpuError flag) noexcept {
  return (ToUnderlying(value) & ToUnderlying(flag)) != 0;
}

inline void PrintSpuError(SpuError error) {
  if (error == SpuError::kOk) {
    return;
  }

  std::cerr << "[SPU] ";

  if (HasFlag(error, SpuError::kNullStruct)) {
    std::cerr << "null structure; ";
  }
  if (HasFlag(error, SpuError::kFileOpen)) {
    std::cerr << "cannot open file; ";
  }
  if (HasFlag(error, SpuError::kParseError)) {
    std::cerr << "parse error; ";
  }
  if (HasFlag(error, SpuError::kWrongVersion)) {
    std::cerr << "wrong bytecode version; ";
  }
  if (HasFlag(error, SpuError::kBytecodeOverflow)) {
    std::cerr << "bytecode overflow; ";
  }
  std::cerr << '\n';
}

inline void PrintRuntimeError(RuntimeError error) {
  if (error == RuntimeError::kOk) {
    return;
  }

  std::cerr << "[RUNTIME] ";

  if (HasFlag(error, RuntimeError::kNullStruct)) {
    std::cerr << "null structure; ";
  }
  if (HasFlag(error, RuntimeError::kMissingArgument)) {
    std::cerr << "missing argument; ";
  }
  if (HasFlag(error, RuntimeError::kStackUnderflow)) {
    std::cerr << "stack underflow; ";
  }
  if (HasFlag(error, RuntimeError::kDivisionByZero)) {
    std::cerr << "division by zero; ";
  }
  if (HasFlag(error, RuntimeError::kSqrtNegativeArgument)) {
    std::cerr << "sqrt of negative value; ";
  }
  if (HasFlag(error, RuntimeError::kInvalidInput)) {
    std::cerr << "invalid input; ";
  }
  if (HasFlag(error, RuntimeError::kUnknownBytecode)) {
    std::cerr << "unknown bytecode; ";
  }
  if (HasFlag(error, RuntimeError::kJmpArgumentIsNegative)) {
    std::cerr << "JMP argument is negative; ";
  }
  if (HasFlag(error, RuntimeError::kJmpArgumentOutOfRange)) {
    std::cerr << "JMP target out of range; ";
  }
  if (HasFlag(error, RuntimeError::kCallArgumentIsNegative)) {
    std::cerr << "CALL argument is negative; ";
  }
  if (HasFlag(error, RuntimeError::kCallArgumentOutOfRange)) {
    std::cerr << "CALL target out of range; ";
  }
  if (HasFlag(error, RuntimeError::kRetValueIsNegative)) {
    std::cerr << "RET returned negative address; ";
  }
  if (HasFlag(error, RuntimeError::kInvalidRegister)) {
    std::cerr << "invalid register; ";
  }
  if (HasFlag(error, RuntimeError::kInvalidRamAddress)) {
    std::cerr << "invalid RAM address; ";
  }

  std::cerr << '\n';
}

}  // namespace spu
