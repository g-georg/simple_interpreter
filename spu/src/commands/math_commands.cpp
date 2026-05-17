#include "commands/math_commands.h"

#include <cmath>

#include "common/error_codes.h"
#include "core/spu.h"

namespace spu {

RuntimeError AddCommand::Execute(Spu& spu) const {
  int32_t lhs = 0;
  int32_t rhs = 0;
  RuntimeError status = FetchOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(lhs + rhs);
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError SubCommand::Execute(Spu& spu) const {
  int32_t lhs = 0;
  int32_t rhs = 0;
  RuntimeError status = FetchOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(lhs - rhs);
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError MulCommand::Execute(Spu& spu) const {
  int32_t lhs = 0;
  int32_t rhs = 0;
  RuntimeError status = FetchOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  spu.PushData(lhs * rhs);
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError DivCommand::Execute(Spu& spu) const {
  int32_t lhs = 0;
  int32_t rhs = 0;
  RuntimeError status = FetchOperands(spu, lhs, rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }
  if (rhs == 0) {
    return RuntimeError::kDivisionByZero;
  }

  spu.PushData(lhs / rhs);
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

RuntimeError SqrtCommand::Execute(Spu& spu) const {
  RuntimeError status = CheckStackSize(spu, 1);
  if (status != RuntimeError::kOk) {
    return status;
  }

  int32_t value = 0;
  status = spu.PopData(value);
  if (status != RuntimeError::kOk) {
    return status;
  }
  if (value < 0) {
    return RuntimeError::kSqrtNegativeArgument;
  }

  spu.PushData(static_cast<int32_t>(std::sqrt(value)));
  spu.AdvanceInstructionPointer();

  return RuntimeError::kOk;
}

}  // namespace spu