#pragma once

#include <cstdint>
#include <string>

namespace assembler {

enum class AssemblerError : uint32_t {
  kOk                     = 0,
  kUnknownCommand         = 1 << 0,
  kMissingArgument        = 1 << 1,
  kWrongArgumentType      = 1 << 2,
  kTrashSymbols           = 1 << 3,
  kInvalidRegisterName    = 1 << 4,
  kInvalidRegisterAddress = 1 << 5,
  kDuplicateLabel         = 1 << 6,
  kInvalidLabel           = 1 << 7,
  kTrashAfterCommand      = 1 << 8,
  kInternalError          = 1u << 31,
};

inline AssemblerError operator|(AssemblerError a, AssemblerError b) {
  return static_cast<AssemblerError>(
      static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

class AssemblerErrorHandler {
 public:
  [[nodiscard]] bool Ok() const { return error_bits_ == 0; }

  [[nodiscard]] bool HasError(AssemblerError error) const {
    return (error_bits_ & ToBits(error)) != 0;
  }

  void AddError(AssemblerError error) {
    error_bits_ |= ToBits(error);
  }

  void Clear() { error_bits_ = 0; }

  [[nodiscard]] uint32_t RawBits() const { return error_bits_; }

  [[nodiscard]] std::string ToString() const {
    if (Ok()) return "OK";

    std::string result;

    if (HasError(AssemblerError::kUnknownCommand))
      result += "Unknown command\n";
    if (HasError(AssemblerError::kMissingArgument))
      result += "Missing argument\n";
    if (HasError(AssemblerError::kWrongArgumentType))
      result += "Wrong argument type\n";
    if (HasError(AssemblerError::kTrashSymbols))
      result += "Unexpected symbols after command\n";
    if (HasError(AssemblerError::kInvalidRegisterName))
      result += "Invalid register name — expected e.g. RAX\n";
    if (HasError(AssemblerError::kInvalidRegisterAddress))
      result += "Invalid memory address register — expected e.g. [RAX]\n";
    if (HasError(AssemblerError::kDuplicateLabel))
      result += "Duplicate label definition\n";
    if (HasError(AssemblerError::kInvalidLabel))
      result += "Invalid or undefined label\n";
    if (HasError(AssemblerError::kTrashAfterCommand))
      result += "Unexpected symbols after command\n";
    if (HasError(AssemblerError::kInternalError))
      result += "Internal assembler error\n";

    return result;
  }

 private:
  static constexpr uint32_t ToBits(AssemblerError error) {
    return static_cast<uint32_t>(error);
  }

  uint32_t error_bits_ = 0;
};

inline bool IsSuccess(AssemblerError error) {
  return error == AssemblerError::kOk;
}

}