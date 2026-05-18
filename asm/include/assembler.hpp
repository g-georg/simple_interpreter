#pragma once

#include "assembler_errors.hpp"
#include "assembler_types.hpp"
#include "command_table.hpp"

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace assembler {

class Assembler {
 public:
  Assembler() = default;

  AssemblerErrorHandler ReadSourceFile(const std::filesystem::path& path);
  AssemblerErrorHandler AssembleProgram();
  AssemblerErrorHandler WriteBinaryOutput(const std::filesystem::path& path) const;

 private:
  AssemblerError RunFirstPass();
  AssemblerError RunFinalPass();

  AssemblerError ProcessLine(std::string_view line, AssemblyPass pass);

  const Command*  LookupCommand(std::string_view& line) const;
  AssemblerError  ParseArgument(std::string_view& line, Argument& arg);

  AssemblerError  RegisterLabel(std::string_view line);
  int             LookupLabel(std::string_view name) const;

  static std::string_view Trim(std::string_view s);
  static std::string_view NextToken(std::string_view& line);

  std::vector<std::string>             lines_;
  std::filesystem::path                source_file_path_;
  std::vector<int>                     bytecode_;
  size_t                               instruction_pointer_ = 0;
  std::unordered_map<std::string, int> labels_;
  size_t                               current_line_number_ = 0;
};

}