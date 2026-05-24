#pragma once

#include "commands/command_base.h"

namespace spu {

class AddCommand final : public BinaryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class SubCommand final : public BinaryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class MulCommand final : public BinaryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class DivCommand final : public BinaryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class SqrtCommand final : public StackCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

}  // namespace spu