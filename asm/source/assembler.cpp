#include "assembler.hpp"
#include "debug.hpp"
#include "file.h"
#include "utils.h"
#include "spu_common.h"

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace assembler {

namespace {
constexpr char kListingFileName[] = "listing.txt";
}

Assembler::~Assembler() {
  TextDtor(&source_text_);

  if (listing_file_) {
    fclose(listing_file_);
    listing_file_ = nullptr;
  }
}

AssemblerErrorHandler Assembler::ReadSourceFile(std::string_view input_file_name) {
  AssemblerErrorHandler handler;

  listing_file_ = fopen(kListingFileName, "w");
  if (!listing_file_) {
    perror("Error opening listing file");
    handler.AddError(AssemblerError::kInternalError);
    return handler;
  }

  for (auto& label : labels_) {
    label.address_value = kUninitializedLabelValue;
    label.hash          = 0;
  }

  source_file_name_ = std::string(input_file_name);

  if (TextCtor(source_file_name_.c_str(), &source_text_) != 0) {
    handler.AddError(AssemblerError::kInternalError);
    return handler;
  }

  StrReplace(source_text_.buffer, "\n;", '\0');

  return handler;
}

AssemblerErrorHandler Assembler::AssembleProgram() {
  AssemblerErrorHandler handler;

  AssemblerError status = RunFirstPass();
  if (!IsSuccess(status)) {
    handler.AddError(status);
    return handler;
  }

  bytecode_.assign(instructions_capacity_, 0);

  status = RunFinalPass();
  if (!IsSuccess(status)) {
    handler.AddError(status);
    return handler;
  }

  return handler;
}

AssemblerErrorHandler Assembler::WriteBinaryOutput(std::string_view output_file_name) {
  AssemblerErrorHandler handler;

  FILE* output_file = fopen(std::string(output_file_name).c_str(), "w");
  if (!output_file) {
    perror("Error opening output file");
    handler.AddError(AssemblerError::kInternalError);
    return handler;
  }

  AssemblerError status = WriteFileHeader(output_file);
  if (!IsSuccess(status)) {
    handler.AddError(status);
  }

  status = WriteFileBody(output_file);
  if (!IsSuccess(status)) {
    handler.AddError(status);
  }

  fclose(output_file);
  return handler;
}

AssemblerError Assembler::RunFirstPass() {
  DEBUG_PRINT("%s", "========== FIRST PASS ==========\n");

  for (size_t i = 0; source_text_.lines[i].start != nullptr; i++) {
    AssemblerError status = ProcessLine(i, AssemblyPass::kFirst);
    if (!IsSuccess(status)) return status;
  }

  instructions_capacity_ = instruction_pointer_ + 1;
  return AssemblerError::kOk;
}

AssemblerError Assembler::RunFinalPass() {
  DEBUG_PRINT("%s", "========== FINAL PASS ==========\n");

  instruction_pointer_ = 0;

  for (size_t i = 0; source_text_.lines[i].start != nullptr; i++) {
    AssemblerError status = ProcessLine(i, AssemblyPass::kFinal);
    if (!IsSuccess(status)) return status;
  }

  return AssemblerError::kOk;
}

AssemblerError Assembler::ProcessLine(size_t line_idx, AssemblyPass pass) {
  current_line_number_ = line_idx + 1;

  char* line_ptr = source_text_.lines[line_idx].start;
  line_ptr = SkipSpaces(line_ptr);

  if (line_ptr[0] == '\0') return AssemblerError::kOk;

  if (line_ptr[0] == ':') {
    AssemblerError status = RegisterLabel(&line_ptr);
    if (!IsSuccess(status)) return status;
    return CheckForTrailingGarbage(line_ptr);
  }

  const Command* command = LookupCommand(&line_ptr);
  if (!command) {
    fprintf(stderr, "%s:%zu Unknown command \"%s\"\n",
            source_file_name_.c_str(), current_line_number_, line_ptr);
    return AssemblerError::kUnknownCommand;
  }

  if (pass == AssemblyPass::kFirst) {
    instruction_pointer_ += 1;
    instruction_pointer_ +=
        (command->allowed_argument_types != ArgumentType::kNone);
    return AssemblerError::kOk;
  }

  const size_t instruction_start = instruction_pointer_;

  AssemblerError status = EmitCommand(command, &line_ptr);
  if (!IsSuccess(status)) return status;

  status = CheckForTrailingGarbage(line_ptr);
  if (!IsSuccess(status)) return status;

  return EmitListingLine(instruction_start);
}

AssemblerError Assembler::EmitCommand(const Command* command, char** line_ptr) {
  assert(command);
  assert(line_ptr && *line_ptr);

  AssemblerError status = GrowBytecodeIfNeeded();
  if (!IsSuccess(status)) return status;

  bytecode_[instruction_pointer_] = command->bytecode;
  instruction_pointer_++;

  if (command->allowed_argument_types == ArgumentType::kNone)
    return AssemblerError::kOk;

  Argument argument;
  status = ParseArgument(&argument, line_ptr);
  if (!IsSuccess(status)) {
    fprintf(stderr, "%s:%zu Missing argument for command \"%s\"\n",
            source_file_name_.c_str(), current_line_number_,
            std::string(command->name).c_str());
    return status;
  }

  if (argument.type != command->allowed_argument_types) {
    fprintf(stderr, "%s:%zu Wrong argument type for \"%s\"\n",
            source_file_name_.c_str(), current_line_number_,
            std::string(command->name).c_str());
    return AssemblerError::kWrongArgumentType;
  }

  bytecode_[instruction_pointer_] = argument.value;
  instruction_pointer_++;

  return AssemblerError::kOk;
}

AssemblerError Assembler::EmitListingLine(size_t instruction_start) {
  if (fprintf(listing_file_, "%04zu \t ", instruction_start) < 0)
    return AssemblerError::kInternalError;

  size_t space_align = 4 + 1 + 4 + 1;

  for (size_t i = instruction_start; i < instruction_pointer_; i++) {
    int written = fprintf(listing_file_, "%4d ", bytecode_[i]);
    if (written < 0) return AssemblerError::kInternalError;
    space_align -= static_cast<size_t>(written);
  }

  if (PrintSymbols(listing_file_, space_align, ' ') != 0)
    return AssemblerError::kInternalError;

  const char* source_line =
      source_text_.lines[current_line_number_ - 1].start;
  if (fprintf(listing_file_, "\t %s\n", source_line) < 0)
    return AssemblerError::kInternalError;

  return AssemblerError::kOk;
}

AssemblerError Assembler::ParseArgument(Argument* argument, char** line_ptr) {
  assert(argument);
  assert(line_ptr && *line_ptr);

  *line_ptr = SkipSpaces(*line_ptr);

  if ((*line_ptr)[0] == ':') {
    *line_ptr += 1;

    argument->type  = ArgumentType::kLabel;
    argument->value = kUninitializedLabelValue;

    size_t word_len       = GetWordLen(*line_ptr, " ");
    (*line_ptr)[word_len] = '\0';
    std::string_view label_name(*line_ptr, word_len);
    *line_ptr += word_len + 1;

    unsigned long label_hash = HashDjb2(label_name.data());
    int idx = LookupLabel(label_name, label_hash);

    if (idx < 0) {
      fprintf(stderr, "%s:%zu Undefined label \":% *s\"\n",
              source_file_name_.c_str(), current_line_number_,
              static_cast<int>(word_len), label_name.data());
      return AssemblerError::kInvalidLabel;
    }

    argument->value = static_cast<int>(labels_[idx].address_value);
    return AssemblerError::kOk;
  }

  int bytes_consumed = 0;
  if (sscanf(*line_ptr, "%d%n", &argument->value, &bytes_consumed) == 1) {
    *line_ptr     += bytes_consumed;
    argument->type = ArgumentType::kNumber;
    return AssemblerError::kOk;
  }

  if (IsSuccess(DecodeRegister(*line_ptr, &bytes_consumed, &argument->value))) {
    *line_ptr     += bytes_consumed;
    argument->type = ArgumentType::kRegister;
    return AssemblerError::kOk;
  }

  if (IsSuccess(DecodeMemoryRegister(*line_ptr, &bytes_consumed, &argument->value))) {
    *line_ptr     += bytes_consumed;
    argument->type = ArgumentType::kMemoryAddress;
    return AssemblerError::kOk;
  }

  argument->type  = ArgumentType::kNone;
  argument->value = -1337;
  return AssemblerError::kMissingArgument;
}

AssemblerError Assembler::RegisterLabel(char** line_ptr) {
  assert(line_ptr && *line_ptr);

  if ((*line_ptr)[0] != ':') {
    fprintf(stderr, "%s:%zu Expected ':' before label name\n",
            source_file_name_.c_str(), current_line_number_);
    return AssemblerError::kInvalidLabel;
  }
  *line_ptr += 1;

  if (labels_count_ >= SPU_MAX_LABELS_COUNT) {
    fprintf(stderr, "%s:%zu Max label count exceeded\n",
            source_file_name_.c_str(), current_line_number_);
    return AssemblerError::kInvalidLabel;
  }

  size_t word_len       = GetWordLen(*line_ptr, " ");
  (*line_ptr)[word_len] = '\0';
  std::string_view label_name(*line_ptr, word_len);
  *line_ptr += word_len;

  unsigned long label_hash = HashDjb2(label_name.data());
  int idx = LookupLabel(label_name, label_hash);

  if (idx >= 0) {
    labels_[idx].name          = label_name;
    labels_[idx].hash          = label_hash;
    labels_[idx].address_value = static_cast<ssize_t>(instruction_pointer_);
  } else {
    labels_[labels_count_].name          = label_name;
    labels_[labels_count_].hash          = label_hash;
    labels_[labels_count_].address_value = static_cast<ssize_t>(instruction_pointer_);
    labels_count_++;
  }

  return AssemblerError::kOk;
}

int Assembler::LookupLabel(std::string_view name, unsigned long hash) const {
  for (size_t i = 0; i < SPU_MAX_LABELS_COUNT; i++) {
    if (labels_[i].hash == hash && labels_[i].name == name) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

const Command* Assembler::LookupCommand(char** line_ptr) const {
  assert(line_ptr && *line_ptr);

  size_t word_len = GetWordLen(*line_ptr, " ");

  for (size_t i = 0; i < kCommandsCount; i++) {
    if (kCommandsTable[i].name.size() != word_len) continue;

    if (strncmp(*line_ptr, kCommandsTable[i].name.data(), word_len) == 0) {
      *line_ptr += word_len + 1;
      return &kCommandsTable[i];
    }
  }

  return nullptr;
}

AssemblerError Assembler::DecodeRegister(char* name, int* bytes_consumed,
                                          int* bytecode) const {
  assert(name && bytes_consumed && bytecode);

  if (strlen(name) < REGISTER_NAME_LEN) return AssemblerError::kInvalidRegisterName;
  if (name[0] != 'R')                   return AssemblerError::kInvalidRegisterName;
  if (name[2] != 'X')                   return AssemblerError::kInvalidRegisterName;

  int register_index = name[1] - 'A';

  if (register_index < 0)                                    return AssemblerError::kInvalidRegisterName;
  if (static_cast<size_t>(register_index) >= NUMBER_OF_REGISTERS) return AssemblerError::kInvalidRegisterName;

  *bytecode      = register_index;
  *bytes_consumed = REGISTER_NAME_LEN + 2;

  return AssemblerError::kOk;
}

AssemblerError Assembler::DecodeMemoryRegister(char* name, int* bytes_consumed,
                                                int* bytecode) const {
  assert(name && bytes_consumed && bytecode);

  if (strlen(name) < REGISTER_NAME_LEN + 2) return AssemblerError::kInvalidRegisterAddress;
  if (name[0] != '[')                        return AssemblerError::kInvalidRegisterAddress;
  if (name[4] != ']')                        return AssemblerError::kInvalidRegisterAddress;

  int inner_bytes = 0;
  AssemblerError status = DecodeRegister(name + 1, &inner_bytes, bytecode);
  if (!IsSuccess(status)) return status;

  *bytes_consumed = inner_bytes + 2;
  return AssemblerError::kOk;
}

AssemblerError Assembler::WriteFileHeader(FILE* output_file) const {
  assert(output_file);

  if (fprintf(output_file, "%lu ", MY_ASM_VERSION) < 0)
    return AssemblerError::kInternalError;

  if (fprintf(output_file, "%zu ", instructions_capacity_ - 1) < 0)
    return AssemblerError::kInternalError;

  return AssemblerError::kOk;
}

AssemblerError Assembler::WriteFileBody(FILE* output_file) const {
  assert(output_file);

  for (size_t i = 0; i < instructions_capacity_; i++) {
    if (fprintf(output_file, "%d ", bytecode_[i]) < 0)
      return AssemblerError::kInternalError;
  }

  return AssemblerError::kOk;
}

AssemblerError Assembler::CheckForTrailingGarbage(char* line_ptr) const {
  const line_t& line = source_text_.lines[current_line_number_ - 1];

  while (*line_ptr != '\0' && line_ptr < line.start + line.len) {
    if (!isspace(static_cast<unsigned char>(*line_ptr))) {
      fprintf(stderr, "%s:%zu Unexpected trailing symbols: \"%s\"\n",
              source_file_name_.c_str(), current_line_number_, line_ptr);
      return AssemblerError::kTrashSymbols;
    }
    line_ptr++;
  }

  return AssemblerError::kOk;
}

AssemblerError Assembler::GrowBytecodeIfNeeded() {
  if (instruction_pointer_ + 1 < instructions_capacity_)
    return AssemblerError::kOk;

  instructions_capacity_ *= 2;
  bytecode_.resize(instructions_capacity_, 0);

  return AssemblerError::kOk;
}

}