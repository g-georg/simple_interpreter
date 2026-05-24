#include "dispatcher/command_dispatcher.h"

#include "commands/command_base.h"
#include "commands/jump_commands.h"
#include "commands/math_commands.h"
#include "commands/stack_commands.h"

namespace spu {

const std::array<std::unique_ptr<ICommand>,
                 CommandDispatcher::kCommandCount>&
CommandDispatcher::Table() {
  static const auto table = [] {
    std::array<std::unique_ptr<ICommand>, kCommandCount> result{};

    result[static_cast<size_t>(Opcode::kPush)] =
        std::make_unique<PushCommand>();
    result[static_cast<size_t>(Opcode::kPop)] = std::make_unique<PopCommand>();
    result[static_cast<size_t>(Opcode::kAdd)] = std::make_unique<AddCommand>();
    result[static_cast<size_t>(Opcode::kSub)] = std::make_unique<SubCommand>();
    result[static_cast<size_t>(Opcode::kDiv)] = std::make_unique<DivCommand>();
    result[static_cast<size_t>(Opcode::kMul)] = std::make_unique<MulCommand>();
    result[static_cast<size_t>(Opcode::kSqrt)] =
        std::make_unique<SqrtCommand>();
    result[static_cast<size_t>(Opcode::kOut)] = std::make_unique<OutCommand>();
    result[static_cast<size_t>(Opcode::kIn)] = std::make_unique<InCommand>();
    result[static_cast<size_t>(Opcode::kJmp)] = std::make_unique<JmpCommand>();
    result[static_cast<size_t>(Opcode::kJb)] = std::make_unique<JbCommand>();
    result[static_cast<size_t>(Opcode::kJbe)] = std::make_unique<JbeCommand>();
    result[static_cast<size_t>(Opcode::kJa)] = std::make_unique<JaCommand>();
    result[static_cast<size_t>(Opcode::kJae)] = std::make_unique<JaeCommand>();
    result[static_cast<size_t>(Opcode::kJe)] = std::make_unique<JeCommand>();
    result[static_cast<size_t>(Opcode::kJne)] = std::make_unique<JneCommand>();
    result[static_cast<size_t>(Opcode::kCall)] =
        std::make_unique<CallCommand>();
    result[static_cast<size_t>(Opcode::kRet)] = std::make_unique<RetCommand>();
    result[static_cast<size_t>(Opcode::kPushm)] =
        std::make_unique<PushmCommand>();
    result[static_cast<size_t>(Opcode::kPopm)] =
        std::make_unique<PopmCommand>();
    result[static_cast<size_t>(Opcode::kDraw)] =
        std::make_unique<DrawCommand>();
    result[static_cast<size_t>(Opcode::kPushr)] =
        std::make_unique<PushrCommand>();
    result[static_cast<size_t>(Opcode::kPopr)] =
        std::make_unique<PoprCommand>();

    return result;
  }();

  return table;
}

RuntimeError CommandDispatcher::Dispatch(Spu& spu, Opcode opcode) {
  size_t index = static_cast<size_t>(opcode);
  if (index >= kCommandCount) {
    return RuntimeError::kUnknownBytecode;
  }

  const std::unique_ptr<ICommand>& command = Table()[index];
  if (command == nullptr) {
    return RuntimeError::kUnknownBytecode;
  }

  return command->Execute(spu);
}

}  // namespace spu