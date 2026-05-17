#pragma once

#include "command_base.h"

namespace spu {

class AddCommand final : public BinaryCommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class SubCommand final : public BinaryCommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class MulCommand final : public BinaryCommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

class DivCommand final : public BinaryCommand {
 public:
  RuntimeError Execute(Spu& spu) const;
};

}

#endif