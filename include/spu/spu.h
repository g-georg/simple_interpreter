#ifndef INCLUDE_SPU_SPU_H_
#define INCLUDE_SPU_SPU_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <stack>
#include <vector>

#include "common/constants.h"
#include "common/error_codes.h"
#include "common/opcodes.h"

namespace spu {

class Spu;

using CommandHandler = std::function<RuntimeError(Spu*)>;

class Spu {
 public:
  Spu();
  ~Spu();

  Spu(const Spu&) = delete;
  Spu& operator=(const Spu&) = delete;
  Spu(Spu&&) noexcept = default;
  Spu& operator=(Spu&&) noexcept = default;

  [[nodiscard]] SpuError LoadBytecode(const char* filename);
  [[nodiscard]] RuntimeError Run();
  void Reset();

  [[nodiscard]] int32_t GetRegister(size_t index) const;
  [[nodiscard]] int32_t GetRam(size_t address) const;
  [[nodiscard]] size_t GetInstructionPointer() const;
  [[nodiscard]] const std::vector<int32_t>& GetBytecode() const;

  [[nodiscard]] std::stack<int32_t>& GetDataStack();
  [[nodiscard]] std::stack<int32_t>& GetReturnStack();
  [[nodiscard]] const std::stack<int32_t>& GetDataStack() const;
  [[nodiscard]] const std::stack<int32_t>& GetReturnStack() const;

 private:
  std::stack<int32_t> data_stack_;
  std::stack<int32_t> return_stack_;

  std::vector<int32_t> bytecode_;
  size_t instruction_pointer_ = 0;

  std::array<int32_t, kRegisterCount> registers_{};
  std::array<int32_t, kMaxRamSize> ram_{};

  std::array<CommandHandler, static_cast<size_t>(kMaxOpcodeValue) + 2> command_table_{};

  [[nodiscard]] RuntimeError FetchOperands(int32_t* first, int32_t* second);
  [[nodiscard]] bool IsValidRamAddress(size_t address) const;
  void InitCommandTable();
};

}

#endif