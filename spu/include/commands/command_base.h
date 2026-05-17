#pragma once

#include <stddef.h>
#include <stdint.h>

#include "common/error_codes.h"

namespace spu {

class Spu;

class ICommand {
 public:
  virtual ~ICommand() = default;

  [[nodiscard]] virtual RuntimeError Execute(Spu& spu) const = 0;
};

class StackCommand : public ICommand {
 protected:
  [[nodiscard]] static RuntimeError CheckStackSize(const Spu& spu,
                                                   size_t required);
};

class BinaryCommand : public StackCommand {
 protected:
  [[nodiscard]] static RuntimeError FetchOperands(Spu& spu,
                                                  int32_t& lhs,
                                                  int32_t& rhs);
};

}  // namespace spu