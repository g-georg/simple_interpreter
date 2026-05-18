#pragma once

#include <array>
#include <stddef.h>
#include <stdint.h>
#include <vector>

#include "common/constants.h"
#include "common/error_codes.h"
#include "containers/stack.h"

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
  void SetBytecode(std::vector<int32_t> bytecode);

  [[nodiscard]] RuntimeError Run();
  void Reset();

  [[nodiscard]] size_t GetInstructionPointer() const noexcept;
  void SetInstructionPointer(size_t instruction_pointer) noexcept;
  void AdvanceInstructionPointer(size_t delta = 1) noexcept;

  [[nodiscard]] const std::vector<int32_t>& GetBytecode() const noexcept;
  [[nodiscard]] size_t GetBytecodeSize() const noexcept;

  [[nodiscard]] bool IsValidInstructionIndex(size_t index) const noexcept;
  [[nodiscard]] bool IsValidRegisterIndex(size_t index) const noexcept;
  [[nodiscard]] bool IsValidRamAddress(size_t address) const noexcept;

  [[nodiscard]] int32_t GetRegister(size_t index) const;
  [[nodiscard]] int32_t GetRam(size_t address) const;

  [[nodiscard]] RuntimeError TryReadOperand(int32_t& operand) const;

  [[nodiscard]] RuntimeError ReadRegister(size_t index, int32_t& value) const;
  [[nodiscard]] RuntimeError WriteRegister(size_t index, int32_t value);

  [[nodiscard]] RuntimeError ReadRam(size_t address, int32_t& value) const;
  [[nodiscard]] RuntimeError WriteRam(size_t address, int32_t value);

  [[nodiscard]] RuntimeError PopData(int32_t& value);
  void PushData(int32_t value);

  [[nodiscard]] RuntimeError PushReturnAddress(size_t value);
  [[nodiscard]] RuntimeError PopReturnAddress(size_t& value);

  [[nodiscard]] Stack<int32_t>& DataStack() noexcept;
  [[nodiscard]] const Stack<int32_t>& DataStack() const noexcept;

  [[nodiscard]] Stack<size_t>& ReturnStack() noexcept;
  [[nodiscard]] const Stack<size_t>& ReturnStack() const noexcept;

  [[nodiscard]] const std::array<int32_t, kRegisterCount>& Registers()
      const noexcept;
  [[nodiscard]] const std::array<int32_t, kMaxRamSize>& Ram() const noexcept;

 private:
  Stack<int32_t> data_stack_;
  Stack<size_t> return_stack_;

  std::vector<int32_t> bytecode_;
  size_t instruction_pointer_ = 0;

  std::array<int32_t, kRegisterCount> registers_{};
  std::array<int32_t, kMaxRamSize> ram_{};
};

}  // namespace spu