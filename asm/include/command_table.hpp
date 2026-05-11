#pragma once

#include "assembler_types.hpp"
#include "spu_common.hpp"

#include <array>
#include <string_view>

namespace assembler {

struct Command {
  std::string_view name;
  spu::CommandCode bytecode;
  ArgumentType allowed_argument_types;
};

constexpr Command MakeCommand(std::string_view name,
                              spu::CommandCode bytecode,
                              ArgumentType args) {
  return {name, bytecode, args};
}

inline constexpr std::array<Command, 24> kCommandsTable({
    MakeCommand("PUSH",  spu::CommandCode::kPush,  ArgumentType::kNumber),
    MakeCommand("POP",   spu::CommandCode::kPop,   ArgumentType::kNone),
    MakeCommand("ADD",   spu::CommandCode::kAdd,   ArgumentType::kNone),
    MakeCommand("SUB",   spu::CommandCode::kSub,   ArgumentType::kNone),
    MakeCommand("DIV",   spu::CommandCode::kDiv,   ArgumentType::kNone),
    MakeCommand("MUL",   spu::CommandCode::kMul,   ArgumentType::kNone),
    MakeCommand("SQRT",  spu::CommandCode::kSqrt,  ArgumentType::kNone),
    MakeCommand("OUT",   spu::CommandCode::kOut,   ArgumentType::kNone),
    MakeCommand("IN",    spu::CommandCode::kIn,    ArgumentType::kNone),
    MakeCommand("JMP",   spu::CommandCode::kJmp,   ArgumentType::kLabel),
    MakeCommand("JB",    spu::CommandCode::kJb,    ArgumentType::kLabel),
    MakeCommand("JBE",   spu::CommandCode::kJbe,   ArgumentType::kLabel),
    MakeCommand("JA",    spu::CommandCode::kJa,    ArgumentType::kLabel),
    MakeCommand("JAE",   spu::CommandCode::kJae,   ArgumentType::kLabel),
    MakeCommand("JE",    spu::CommandCode::kJe,    ArgumentType::kLabel),
    MakeCommand("JNE",   spu::CommandCode::kJne,   ArgumentType::kLabel),
    MakeCommand("CALL",  spu::CommandCode::kCall,  ArgumentType::kLabel),
    MakeCommand("RET",   spu::CommandCode::kRet,   ArgumentType::kNone),
    MakeCommand("PUSHM", spu::CommandCode::kPushM, ArgumentType::kMemoryAddress),
    MakeCommand("POPM",  spu::CommandCode::kPopM,  ArgumentType::kMemoryAddress),
    MakeCommand("PUSHR", spu::CommandCode::kPushR, ArgumentType::kRegister),
    MakeCommand("POPR",  spu::CommandCode::kPopR,  ArgumentType::kRegister),
    MakeCommand("DRAW",  spu::CommandCode::kDraw,  ArgumentType::kNone),
    MakeCommand("HLT",   spu::CommandCode::kHlt,   ArgumentType::kNone),
});

inline constexpr size_t kCommandsCount = kCommandsTable.size();

}