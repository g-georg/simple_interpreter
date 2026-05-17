#pragma once

#include "commands/command_base.h"

namespace spu {

class JmpCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JbCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JbeCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JaCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JaeCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JeCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class JneCommand final : public ICommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

}
