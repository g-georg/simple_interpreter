#ifndef INCLUDE_SPU_COMMANDS_H_
#define INCLUDE_SPU_COMMANDS_H_

#include <functional>

#include "common/error_codes.h"
#include "common/opcodes.h"

namespace spu {

class Spu;

[[nodiscard]] std::function<RuntimeError(Spu*)> GetCommandHandler(Opcode opcode);
[[nodiscard]] const char* OpcodeToString(Opcode opcode);

}

#endif