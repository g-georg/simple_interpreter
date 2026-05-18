#pragma once

#include <array>
#include <memory>
#include <stddef.h>

#include "common/error_codes.h"
#include "enums/opcode.h"

namespace spu {

class Spu;
class ICommand;

class CommandDispatcher {
 public:
  [[nodiscard]] static RuntimeError Dispatch(Spu& spu, Opcode opcode);

 private:
  static constexpr size_t kCommandCount = static_cast<size_t>(Opcode::kCount);

  [[nodiscard]] static const std::array<std::unique_ptr<ICommand>,
                                        kCommandCount>&
  Table();
};

}  // namespace spu