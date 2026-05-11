#include "assembler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace assembler {

AssemblerErrorHandler Assembler::ReadSourceFile(std::string_view filename) {
  AssemblerErrorHandler handler;

  source_file_name_ = std::string(filename);

  std::ifstream file(source_file_name_);
  if (!file) {
    std::cerr << "Failed to open file: " << source_file_name_ << "\n";
    handler.AddError(AssemblerError::kInternalError);
    return handler;
  }

  std::string line;
  while (std::getline(file, line)) {
    auto comment_pos = line.find(';');
    if (comment_pos != std::string::npos)
      line = line.substr(0, comment_pos);

    lines_.push_back(line);
  }

  return handler;
}

AssemblerErrorHandler Assembler::AssembleProgram() {
  AssemblerErrorHandler handler;

  instruction_pointer_ = 0;

  auto status = RunFirstPass();
  if (!IsSuccess(status)) {
    handler.AddError(status);
    return handler;
  }

  instruction_pointer_ = 0;
  
  bytecode_.clear();

  status = RunFinalPass();
  if (!IsSuccess(status)) {
    handler.AddError(status);
    return handler;
  }

  return handler;
}

AssemblerError Assembler::RunFirstPass() {
  instruction_pointer_ = 0;

  for (size_t i = 0; i < lines_.size(); ++i) {
    current_line_number_ = i + 1;
    auto status = ProcessLine(lines_[i], AssemblyPass::kFirst);
    if (!IsSuccess(status)) return status;
  }
  return AssemblerError::kOk;
}

AssemblerError Assembler::RunFinalPass() {
  for (size_t i = 0; i < lines_.size(); ++i) {
    current_line_number_ = i + 1;
    auto status = ProcessLine(lines_[i], AssemblyPass::kFinal);
    if (!IsSuccess(status)) return status;
  }
  return AssemblerError::kOk;
}

AssemblerError Assembler::ProcessLine(std::string_view line, AssemblyPass pass) {
  line = Trim(line);
  if (line.empty()) return AssemblerError::kOk;

  if (line.front() == ':') {
    if (pass == AssemblyPass::kFirst)
      return RegisterLabel(line);
    return AssemblerError::kOk;
  }

  const Command* cmd = LookupCommand(line);
  if (!cmd) {
    std::cerr << source_file_name_ << ":" << current_line_number_
              << " Unknown command\n";
    return AssemblerError::kUnknownCommand;
  }

  if (pass == AssemblyPass::kFirst) {
    instruction_pointer_++;
    if (cmd->allowed_argument_types != ArgumentType::kNone)
      instruction_pointer_++;
    return AssemblerError::kOk;
  }

  bytecode_.push_back(static_cast<int>(cmd->bytecode));

  if (cmd->allowed_argument_types != ArgumentType::kNone) {
    Argument arg;
    auto status = ParseArgument(line, arg);
    if (!IsSuccess(status)) return status;

    if (arg.type != cmd->allowed_argument_types) {
      return AssemblerError::kWrongArgumentType;
    }

    bytecode_.push_back(arg.value);
  }

  return AssemblerError::kOk;
}

const Command* Assembler::LookupCommand(std::string_view& line) const {
  auto token = NextToken(line);

  for (const auto& cmd : kCommandsTable) {
    if (cmd.name == token) {
      return &cmd;
    }
  }
  return nullptr;
}

AssemblerError Assembler::ParseArgument(std::string_view& line, Argument& arg) {
  auto token = NextToken(line);
  if (token.empty()) return AssemblerError::kMissingArgument;

  if (token.front() == ':') {
    auto name = token.substr(1);
    int addr = LookupLabel(name);
    if (addr < 0) return AssemblerError::kInvalidLabel;

    arg.type  = ArgumentType::kLabel;
    arg.value = addr;
    return AssemblerError::kOk;
  }

  try {
    arg.value = std::stoi(std::string(token));
    arg.type  = ArgumentType::kNumber;
    return AssemblerError::kOk;
  } catch (...) {}

  if (token.size() == 3 && token[0] == 'R' && token[2] == 'X') {
    int idx    = token[1] - 'A';
    arg.type   = ArgumentType::kRegister;
    arg.value  = idx;
    return AssemblerError::kOk;
  }

  if (token.size() == 5 && token.front() == '[' && token.back() == ']') {
    auto inner = token.substr(1, 3);
    int idx    = inner[1] - 'A';
    arg.type   = ArgumentType::kMemoryAddress;
    arg.value  = idx;
    return AssemblerError::kOk;
  }

  return AssemblerError::kWrongArgumentType;
}

AssemblerError Assembler::RegisterLabel(std::string_view line) {
  line.remove_prefix(1);
  auto name = NextToken(line);

  labels_[std::string(name)] = static_cast<int>(instruction_pointer_);
  return AssemblerError::kOk;
}

int Assembler::LookupLabel(std::string_view name) const {
  auto it = labels_.find(std::string(name));
  if (it == labels_.end()) return -1;
  return it->second;
}

AssemblerErrorHandler Assembler::WriteBinaryOutput(std::string_view filename) const {
  AssemblerErrorHandler handler;

  std::ofstream out{std::string(filename)};
  if (!out) {
    handler.AddError(AssemblerError::kInternalError);
    return handler;
  }

  out << spu::kAsmVersion << " " << bytecode_.size() << "\n";

  for (int code : bytecode_) {
    out << code << " ";
  }

  return handler;
}

std::string_view Assembler::Trim(std::string_view s) {
  while (!s.empty() && isspace(static_cast<unsigned char>(s.front())))
    s.remove_prefix(1);
  while (!s.empty() && isspace(static_cast<unsigned char>(s.back())))
    s.remove_suffix(1);
  return s;
}

std::string_view Assembler::NextToken(std::string_view& line) {
  line = Trim(line);
  if (line.empty()) return {};

  size_t pos = line.find(' ');
  if (pos == std::string_view::npos) {
    auto token = line;
    line = {};
    return token;
  }

  auto token = line.substr(0, pos);
  line.remove_prefix(pos + 1);
  return token;
}

}