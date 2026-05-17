#pragma once

#include <stddef.h>
#include <stdint.h>

#include "commands/command_base.h"

namespace spu {

class JumpCommandBase : public ICommand {
 protected:
  [[nodiscard]] static RuntimeError ReadTarget(Spu& spu, size_t& target);
  [[nodiscard]] static RuntimeError ValidateTarget(const Spu& spu,
                                                   int32_t raw_target);
};

class ConditionalJumpCommand : public JumpCommandBase {
 protected:
  [[nodiscard]] static RuntimeError FetchConditionOperands(Spu& spu,
                                                           int32_t& lhs,
                                                           int32_t& rhs);
};

class JmpCommand final : public JumpCommandBase {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JbCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JbeCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JaCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JaeCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JeCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class JneCommand final : public ConditionalJumpCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class CallCommand final : public JumpCommandBase {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class RetCommand final : public ICommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

}  // namespace spu