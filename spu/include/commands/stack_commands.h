#pragma once

#include "commands/command_base.h"

namespace spu {

class PushCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class PopCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class PushrCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class PoprCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

}
