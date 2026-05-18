#include "commands/command_base.h"

#include "core/spu.h"

namespace spu {

RuntimeError StackCommand::CheckStackSize(const Spu& spu, size_t required) {
  if (spu.DataStack().Size() < required) {
    return RuntimeError::kStackUnderflow;
  }

  return RuntimeError::kOk;
}

RuntimeError BinaryCommand::FetchOperands(Spu& spu, int32_t& lhs,
                                          int32_t& rhs) {
  RuntimeError status = CheckStackSize(spu, 2);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.PopData(rhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  status = spu.PopData(lhs);
  if (status != RuntimeError::kOk) {
    return status;
  }

  return RuntimeError::kOk;
}

}  // namespace spu