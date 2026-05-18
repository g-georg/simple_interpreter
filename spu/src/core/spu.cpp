#include "core/spu.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <utility>

#include "dispatcher/command_dispatcher.h"

namespace spu {

SpuError Spu::LoadBytecode(const char* filename) {
  if (filename == nullptr) {
    return SpuError::kFileOpen;
  }

  std::ifstream input(filename);
  if (!input.is_open()) {
    return SpuError::kFileOpen;
  }

  uint32_t version = 0;
  size_t bytecode_size = 0;
  if (!(input >> version >> bytecode_size)) {
    return SpuError::kParseError;
  }
  if (version != kAsmVersion) {
    return SpuError::kWrongVersion;
  }

  std::vector<int32_t> bytecode(bytecode_size);
  for (size_t index = 0; index < bytecode_size; ++index) {
    if (!(input >> bytecode[index])) {
      return SpuError::kParseError;
    }
  }

  SetBytecode(std::move(bytecode));

  return SpuError::kOk;
}

void Spu::SetBytecode(std::vector<int32_t> bytecode) {
  bytecode_ = std::move(bytecode);
  instruction_pointer_ = 0;
}

RuntimeError Spu::Run() {
  while (instruction_pointer_ < bytecode_.size()) {
    int32_t raw_opcode = bytecode_[instruction_pointer_];
    if (raw_opcode < 0 || raw_opcode >= static_cast<int32_t>(Opcode::kCount)) {
      return RuntimeError::kUnknownBytecode;
    }

    Opcode opcode = static_cast<Opcode>(raw_opcode);
    if (opcode == Opcode::kHlt) {
      return RuntimeError::kOk;
    }

    RuntimeError status = CommandDispatcher::Dispatch(*this, opcode);
    if (status != RuntimeError::kOk) {
      return status;
    }
  }

  return RuntimeError::kOk;
}

void Spu::Reset() {
  data_stack_.Clear();
  return_stack_.Clear();
  instruction_pointer_ = 0;
  registers_.fill(0);
  ram_.fill(0);
}

size_t Spu::GetInstructionPointer() const noexcept {
  return instruction_pointer_;
}

void Spu::SetInstructionPointer(size_t instruction_pointer) noexcept {
  instruction_pointer_ = instruction_pointer;
}

void Spu::AdvanceInstructionPointer(size_t delta) noexcept {
  instruction_pointer_ += delta;
}

const std::vector<int32_t>& Spu::GetBytecode() const noexcept {
  return bytecode_;
}

size_t Spu::GetBytecodeSize() const noexcept {
  return bytecode_.size();
}

bool Spu::IsValidInstructionIndex(size_t index) const noexcept {
  return index < bytecode_.size();
}

bool Spu::IsValidRegisterIndex(size_t index) const noexcept {
  return index < registers_.size();
}

bool Spu::IsValidRamAddress(size_t address) const noexcept {
  return address < ram_.size();
}

int32_t Spu::GetRegister(size_t index) const {
  if (!IsValidRegisterIndex(index)) {
    return 0;
  }

  return registers_[index];
}

int32_t Spu::GetRam(size_t address) const {
  if (!IsValidRamAddress(address)) {
    return 0;
  }

  return ram_[address];
}

RuntimeError Spu::TryReadOperand(int32_t& operand) const {
  size_t operand_index = instruction_pointer_ + 1;
  if (operand_index >= bytecode_.size()) {
    return RuntimeError::kMissingArgument;
  }

  operand = bytecode_[operand_index];

  return RuntimeError::kOk;
}

RuntimeError Spu::ReadRegister(size_t index, int32_t& value) const {
  if (!IsValidRegisterIndex(index)) {
    return RuntimeError::kInvalidRegister;
  }

  value = registers_[index];

  return RuntimeError::kOk;
}

RuntimeError Spu::WriteRegister(size_t index, int32_t value) {
  if (!IsValidRegisterIndex(index)) {
    return RuntimeError::kInvalidRegister;
  }

  registers_[index] = value;

  return RuntimeError::kOk;
}

RuntimeError Spu::ReadRam(size_t address, int32_t& value) const {
  if (!IsValidRamAddress(address)) {
    return RuntimeError::kInvalidRamAddress;
  }

  value = ram_[address];

  return RuntimeError::kOk;
}

RuntimeError Spu::WriteRam(size_t address, int32_t value) {
  if (!IsValidRamAddress(address)) {
    return RuntimeError::kInvalidRamAddress;
  }

  ram_[address] = value;

  return RuntimeError::kOk;
}

RuntimeError Spu::PopData(int32_t& value) {
  try {
    value = data_stack_.Pop();
  } catch (const std::underflow_error&) {
    return RuntimeError::kStackUnderflow;
  }

  return RuntimeError::kOk;
}

void Spu::PushData(int32_t value) {
  data_stack_.Push(value);
}

RuntimeError Spu::PushReturnAddress(size_t value) {
  return_stack_.Push(value);

  return RuntimeError::kOk;
}

RuntimeError Spu::PopReturnAddress(size_t& value) {
  try {
    value = return_stack_.Pop();
  } catch (const std::underflow_error&) {
    return RuntimeError::kStackUnderflow;
  }

  return RuntimeError::kOk;
}

Stack<int32_t>& Spu::DataStack() noexcept {
  return data_stack_;
}

const Stack<int32_t>& Spu::DataStack() const noexcept {
  return data_stack_;
}

Stack<size_t>& Spu::ReturnStack() noexcept {
  return return_stack_;
}

const Stack<size_t>& Spu::ReturnStack() const noexcept {
  return return_stack_;
}

const std::array<int32_t, kRegisterCount>& Spu::Registers() const noexcept {
  return registers_;
}

const std::array<int32_t, kMaxRamSize>& Spu::Ram() const noexcept {
  return ram_;
}

}  // namespace spu