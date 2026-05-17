#pragma once

#include <array>
#include <memory>

#include "common/opcodes.h"
#include "common/error_codes.h"

namespace spu {

class Spu;
class ICommand;

class CommandDispatcher {
 public:
  static RuntimeError Dispatch(Spu& spu, Opcode opcode);

 private:
  static constexpr std::size_t kCommandCount =
      static_cast<std::size_t>(Opcode::kCount);

  static const std::array<std::unique_ptr<ICommand>, kCommandCount> table_;
};

}