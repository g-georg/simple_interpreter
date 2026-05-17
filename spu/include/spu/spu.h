#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <std::stack>
#include <vector>

#include "common/constants.h"
#include "common/error_codes.h"
#include "common/opcodes.h"

namespace spu {

class Spu {
 public:
  Spu() = default;
  ~Spu() = default;

  Spu(const Spu&) = delete;
  Spu& operator=(const Spu&) = delete;
  Spu(Spu&&) noexcept = default;
  Spu& operator=(Spu&&) noexcept = default;

  [[nodiscard]] SpuError LoadBytecode(const char* filename);
  [[nodiscard]] RuntimeError Run();
  void Reset();

  [[nodiscard]] size_t GetInstructionPointer() const noexcept;
  void SetInstructionPointer(size_t ip) noexcept;
  void AdvanceInstructionPointer(size_t delta = 1) noexcept;

  [[nodiscard]] const vector<int32_t>& GetBytecode() const noexcept;
  [[nodiscard]] size_t GetBytecodeSize() const noexcept;

  [[nodiscard]] bool IsValidInstructionIndex(size_t index) const noexcept;
  [[nodiscard]] bool IsValidRegisterIndex(size_t index) const noexcept;
  [[nodiscard]] bool IsValidRamAddress(size_t address) const noexcept;

  [[nodiscard]] int32_t GetRegister(size_t index) const;
  [[nodiscard]] int32_t GetRam(size_t address) const;

  [[nodiscard]] RuntimeError ReadRegister(size_t index, int32_t& value) const;
  [[nodiscard]] RuntimeError WriteRegister(size_t index, int32_t value);

  [[nodiscard]] RuntimeError ReadRam(size_t address, int32_t& value) const;
  [[nodiscard]] RuntimeError WriteRam(size_t address, int32_t value);

  [[nodiscard]] RuntimeError TryReadOperand(int32_t& operand) const;

  [[nodiscard]] RuntimeError PushReturnAddress(int32_t value);
  [[nodiscard]] RuntimeError PopReturnAddress(int32_t& value);

  [[nodiscard]] std::stack<int32_t>& Datastd::stack() noexcept;
  [[nodiscard]] std::stack<int32_t>& Returnstd::stack() noexcept;
  [[nodiscard]] const std::stack<int32_t>& Datastd::stack() const noexcept;
  [[nodiscard]] const std::stack<int32_t>& Returnstd::stack() const noexcept;

 private:
  std::stack<int32_t> data_std::stack_;
  std::stack<int32_t> return_std::stack_;

  vector<int32_t> bytecode_;
  size_t instruction_pointer_ = 0;

  array<int32_t, kRegisterCount> registers_{};
  array<int32_t, kMaxRamSize> ram_{};
};

}  // namespace spu
