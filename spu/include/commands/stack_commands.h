#pragma once

#include <stddef.h>
#include <stdint.h>

#include "commands/command_base.h"

namespace spu {

class RegisterCommand : public StackCommand {
 protected:
  [[nodiscard]] static RuntimeError ReadRegisterIndex(Spu& spu,
                                                      size_t& register_index);
};

class MemoryCommand : public RegisterCommand {
 protected:
  [[nodiscard]] static RuntimeError ReadRamAddressFromRegister(
      Spu& spu,
      size_t& address);
};

class IoCommand : public ICommand {
 protected:
  IoCommand() = default;
};

class PushCommand final : public StackCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class PopCommand final : public StackCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class PushrCommand final : public RegisterCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class PoprCommand final : public RegisterCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class PushmCommand final : public MemoryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class PopmCommand final : public MemoryCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class InCommand final : public IoCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class OutCommand final : public StackCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

class DrawCommand final : public IoCommand {
 public:
  [[nodiscard]] RuntimeError Execute(Spu& spu) const override;
};

}  // namespace spu