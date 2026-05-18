#include "commands/jump_commands.h"

#include "core/spu.h"

namespace spu {

namespace {

RuntimeError ValidateCallTarget(const Spu& spu, int32_t raw_target,
                                size_t& target) {
  if (raw_target < 0) {
    return RuntimeError::kCallArgumentIsNegative;
  }

  target = static_cast<size_t>(raw_target);
  if (!spu.IsValidInstructionIndex(target)) {
    return RuntimeError::kCallArgumentOutOfRange;
  }

  return RuntimeError::kOk;
}

}  // namespace

RuntimeError JumpCommandBase::ValidateTarget(const Spu& spu,
                                             int32_t raw_target) {
  if (raw_target < 0) {
    return RuntimeError::kJmpArgumentIsNegative;
  }

  if (!spu.IsValidInstructionIndex(static_cast<size_t>(raw_target))) {
    return RuntimeError::kJmpArgumentOutOfRange;
  }

  return RuntimeError::kOk;
}

RuntimeError JumpCommandBase::ReadTarget(Spu& spu, size_t& target) {
  int32_t raw_target = 0;
  RuntimeError status = spu.TryReadOperand(raw_target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = ValidateTarget(spu, raw_target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  target = static_cast<size_t>(raw_target);

  return RuntimeError::kOk;
}

RuntimeError ConditionalJumpCommand::FetchConditionOperands(Spu& spu,
                                                            int32_t& lhs,
                                                            int32_t& rhs) {
  if (spu.DataStack().Size() < 2) {
    return RuntimeError::kStackUnderflow;
  }

  RuntimeError status = spu.PopData(rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.PopData(lhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  return RuntimeError::kOk;
}

RuntimeError JmpCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.SetInstructionPointer(target);

  return RuntimeError::kOk;
}

RuntimeError JbCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs < rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError JbeCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs <= rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError JaCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs > rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError JaeCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs >= rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError JeCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs == rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError JneCommand::Execute(Spu& spu) const {
  size_t target = 0;
  RuntimeError status = ReadTarget(spu, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t lhs = 0;
  int32_t rhs = 0;
  status = FetchConditionOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (lhs != rhs) {
    spu.SetInstructionPointer(target);
  } else {
    spu.AdvanceInstructionPointer(2);
  }

  return RuntimeError::kOk;
}

RuntimeError CallCommand::Execute(Spu& spu) const {
  int32_t raw_target = 0;
  RuntimeError status = spu.TryReadOperand(raw_target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  size_t target = 0;
  status = ValidateCallTarget(spu, raw_target, target);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.PushReturnAddress(spu.GetInstructionPointer() + 2);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.SetInstructionPointer(target);

  return RuntimeError::kOk;
}

RuntimeError RetCommand::Execute(Spu& spu) const {
  size_t return_address = 0;
  RuntimeError status = spu.PopReturnAddress(return_address);
  if (status != RuntimeError::kOk) {
    return status;
  }

  if (!spu.IsValidInstructionIndex(return_address)) {
    return RuntimeError::kJmpArgumentOutOfRange;
  }

  spu.SetInstructionPointer(return_address);

  return RuntimeError::kOk;
}

}  // namespace spu