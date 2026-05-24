#pragma once

#include "assembler_types.hpp"
#include "spu_common.hpp"

#include <array>
#include <string>
#include <string_view>

namespace assembler {

struct Command {
  std::string      name;
  spu::Opcode      bytecode;
  ArgumentType     allowed_argument_types;
};

inline Command MakeCommand(std::string_view name,
                           spu::Opcode bytecode,
                           ArgumentType args) {
  return {std::string(name), bytecode, args};
}

inline const std::array<Command, 24> kCommandsTable({
    MakeCommand("PUSH",  spu::Opcode::kPush,  ArgumentType::kNumber),
    MakeCommand("POP",   spu::Opcode::kPop,   ArgumentType::kNone),
    MakeCommand("ADD",   spu::Opcode::kAdd,   ArgumentType::kNone),
    MakeCommand("SUB",   spu::Opcode::kSub,   ArgumentType::kNone),
    MakeCommand("DIV",   spu::Opcode::kDiv,   ArgumentType::kNone),
    MakeCommand("MUL",   spu::Opcode::kMul,   ArgumentType::kNone),
    MakeCommand("SQRT",  spu::Opcode::kSqrt,  ArgumentType::kNone),
    MakeCommand("OUT",   spu::Opcode::kOut,   ArgumentType::kNone),
    MakeCommand("IN",    spu::Opcode::kIn,    ArgumentType::kNone),
    MakeCommand("JMP",   spu::Opcode::kJmp,   ArgumentType::kLabel),
    MakeCommand("JB",    spu::Opcode::kJb,    ArgumentType::kLabel),
    MakeCommand("JBE",   spu::Opcode::kJbe,   ArgumentType::kLabel),
    MakeCommand("JA",    spu::Opcode::kJa,    ArgumentType::kLabel),
    MakeCommand("JAE",   spu::Opcode::kJae,   ArgumentType::kLabel),
    MakeCommand("JE",    spu::Opcode::kJe,    ArgumentType::kLabel),
    MakeCommand("JNE",   spu::Opcode::kJne,   ArgumentType::kLabel),
    MakeCommand("CALL",  spu::Opcode::kCall,  ArgumentType::kLabel),
    MakeCommand("RET",   spu::Opcode::kRet,   ArgumentType::kNone),
    MakeCommand("PUSHM", spu::Opcode::kPushm, ArgumentType::kMemoryAddress),
    MakeCommand("POPM",  spu::Opcode::kPopm,  ArgumentType::kMemoryAddress),
    MakeCommand("DRAW",  spu::Opcode::kDraw,  ArgumentType::kNone),
    MakeCommand("PUSHR", spu::Opcode::kPushr, ArgumentType::kRegister),
    MakeCommand("POPR",  spu::Opcode::kPopr,  ArgumentType::kRegister),
    MakeCommand("HLT",   spu::Opcode::kHlt,   ArgumentType::kNone),
});

inline constexpr size_t kCommandsCount = kCommandsTable.size();

}