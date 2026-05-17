#pragma once

#include "common/error_codes.h"

namespace spu {

class Spu;

class ICommand {
 public:
  virtual ~ICommand() = default;
};

class StackCommand : public ICommand {
 protected:
  static RuntimeError CheckStackSize(
      const Spu& spu,
      std::size_t required);
};

class BinaryCommand : public StackCommand {
 protected:
  static RuntimeError FetchOperands(
      Spu& spu,
      int32_t& lhs,
      int32_t& rhs);
};

}
