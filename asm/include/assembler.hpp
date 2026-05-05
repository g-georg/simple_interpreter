#pragma once

#include "assembler_errors.hpp"
#include "assembler_types.hpp"
#include "command_table.hpp"
#include "file.hpp"
#include "spu_common.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

namespace assembler {

inline constexpr int kUninitializedLabelValue = -1;

struct Label {
  unsigned long    hash          = 0;
  ssize_t          address_value = kUninitializedLabelValue;
  std::string_view name          = {};
};

class Assembler {
 public:
  Assembler()  = default;
  ~Assembler();

  Assembler(const Assembler&)            = delete;
  Assembler& operator=(const Assembler&) = delete;

  AssemblerErrorHandler ReadSourceFile    (std::string_view input_file_name);
  AssemblerErrorHandler AssembleProgram   ();
  AssemblerErrorHandler WriteBinaryOutput (std::string_view output_file_name);

 private:
  AssemblerError RunFirstPass ();
  AssemblerError RunFinalPass ();

  AssemblerError ProcessLine   (size_t line_idx, AssemblyPass pass);
  AssemblerError EmitCommand   (const Command* command, char** line_ptr);
  AssemblerError ParseArgument (Argument* argument,     char** line_ptr);

  AssemblerError RegisterLabel (char** line_ptr);
  int            LookupLabel   (std::string_view name, unsigned long hash) const;

  const Command* LookupCommand (char** line_ptr) const;

  AssemblerError DecodeRegister       (char* name, int* bytes_consumed, int* bytecode) const;
  AssemblerError DecodeMemoryRegister (char* name, int* bytes_consumed, int* bytecode) const;

  AssemblerError WriteFileHeader       (FILE* output_file) const;
  AssemblerError WriteFileBody         (FILE* output_file) const;
  AssemblerError EmitListingLine       (size_t instruction_start);
  AssemblerError CheckForTrailingGarbage (char* line_ptr) const;
  AssemblerError GrowBytecodeIfNeeded  ();

  text_t      source_text_         = {};
  std::string source_file_name_    = {};
  size_t      current_line_number_ = 0;

  std::vector<int> bytecode_              = {};
  size_t           instruction_pointer_   = 0;
  size_t           instructions_capacity_ = 0;

  std::array<Label, SPU_MAX_LABELS_COUNT> labels_       = {};
  size_t                                  labels_count_ = 0;

  FILE* listing_file_ = nullptr;
};

}