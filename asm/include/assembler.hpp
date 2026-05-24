#pragma once

#include "assembler_errors.hpp"
#include "assembler_types.hpp"
#include "command_table.hpp"

#include <concepts>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace assembler {

template<typename T>
concept NamedEntity = requires(T t) {
  { t.name } -> std::convertible_to<std::string_view>;
};

template<NamedEntity T, size_t N>
const T* FindByName(const std::array<T, N>& table, std::string_view name) {
  for (const auto& entry : table) {
    if (entry.name == name) return &entry;
  }
  return nullptr;
}

class Assembler {
 public:
  Assembler() = default;

  AssemblerErrorHandler ReadSourceFile(const std::filesystem::path& path);
  AssemblerErrorHandler AssembleProgram();
  AssemblerErrorHandler WriteBinaryOutput(const std::filesystem::path& path) const;
  AssemblerErrorHandler ReadSourceLines(const std::vector<std::string>& lines);

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