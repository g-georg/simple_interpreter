#include "commands/stack_commands.h"

#include <iostream>

#include "common/constants.h"
#include "core/spu.h"

namespace spu {

RuntimeError RegisterCommand::ReadRegisterIndex(Spu& spu,
                                                size_t& register_index) {
  int32_t raw_register_index = 0;
  RuntimeError status = spu.TryReadOperand(raw_register_index);
  if (status != RuntimeError::kOk) {
    return status;
  }
  if (raw_register_index < 0) {
    return RuntimeError::kInvalidRegister;
  }

  register_index = static_cast<size_t>(raw_register_index);
  if (!spu.IsValidRegisterIndex(register_index)) {
    return RuntimeError::kInvalidRegister;
  }

  return RuntimeError::kOk;
}

RuntimeError MemoryCommand::ReadRamAddressFromRegister(Spu& spu,
                                                       size_t& address) {
  size_t register_index = 0;
  RuntimeError status = ReadRegisterIndex(spu, register_index);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t raw_address = 0;
  status = spu.ReadRegister(register_index, raw_address);
  if (status != RuntimeError::kOk) {
    return status;
  }
  if (raw_address < 0) {
    return RuntimeError::kInvalidRamAddress;
  }

  address = static_cast<size_t>(raw_address);
  if (!spu.IsValidRamAddress(address)) {
    return RuntimeError::kInvalidRamAddress;
  }

  return RuntimeError::kOk;
}

RuntimeError PushCommand::Execute(Spu& spu) const {
  int32_t value = 0;
  RuntimeError status = spu.TryReadOperand(value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(value);
  spu.AdvanceInstructionPointer(2);

  return RuntimeError::kOk;
}

RuntimeError PopCommand::Execute(Spu& spu) const {
  RuntimeError status = CheckStackSize(spu, 1);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.PopData(value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError PushrCommand::Execute(Spu& spu) const {
  size_t register_index = 0;
  RuntimeError status = ReadRegisterIndex(spu, register_index);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.ReadRegister(register_index, value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(value);
  spu.AdvanceInstructionPointer(2);

  return RuntimeError::kOk;
}

RuntimeError PoprCommand::Execute(Spu& spu) const {
  size_t register_index = 0;
  RuntimeError status = ReadRegisterIndex(spu, register_index);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = CheckStackSize(spu, 1);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.PopData(value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.WriteRegister(register_index, value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.AdvanceInstructionPointer(2);

  return RuntimeError::kOk;
}

RuntimeError PushmCommand::Execute(Spu& spu) const {
  size_t address = 0;
  RuntimeError status = ReadRamAddressFromRegister(spu, address);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.ReadRam(address, value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(value);
  spu.AdvanceInstructionPointer(2);

  return RuntimeError::kOk;
}

RuntimeError PopmCommand::Execute(Spu& spu) const {
  size_t address = 0;
  RuntimeError status = ReadRamAddressFromRegister(spu, address);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = CheckStackSize(spu, 1);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.PopData(value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.WriteRam(address, value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.AdvanceInstructionPointer(2);

  return RuntimeError::kOk;
}

RuntimeError InCommand::Execute(Spu& spu) const {
  int32_t value = 0;
  if (!(std::cin >> value)) {
    return RuntimeError::kInvalidInput;
  }

  spu.PushData(value);
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError OutCommand::Execute(Spu& spu) const {
  RuntimeError status = CheckStackSize(spu, 1);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.PopData(value);
  if (status != RuntimeError::kOk) {
    return status;
  }

  std::cout << "OUT: " << value << '\n';
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError DrawCommand::Execute(Spu& spu) const {
  const auto& ram = spu.Ram();

  for (size_t row = 0; row < kVideoHeight; ++row) {
    for (size_t column = 0; column < kVideoWidth; ++column) {
      int32_t value = ram[row * kVideoWidth + column];
      char symbol = value == 0 ? ' ' : static_cast<char>(value);
      std::cout << symbol;
    }
    std::cout << '\n';
  }

  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

}  // namespace spu