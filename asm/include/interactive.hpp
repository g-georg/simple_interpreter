#pragma once

#include "assembler.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace assembler {

class InteractiveMode {
 public:
  void Run() {
    PrintHelp();

    std::vector<std::string> lines;
    std::string input;

    while (true) {
      std::cout << "> ";
      if (!std::getline(std::cin, input)) break;

      if (input == "quit" || input == "exit") {
        std::cout << "Bye.\n";
        break;
      }

      if (input == "run") {
        if (lines.empty()) {
          std::cout << "Nothing to assemble.\n";
          continue;
        }
        RunAssembler(lines);
        lines.clear();
        continue;
      }

      if (input == "clear") {
        lines.clear();
        std::cout << "Cleared.\n";
        continue;
      }

      if (input == "list") {
        if (lines.empty()) {
          std::cout << "No instructions yet.\n";
        } else {
          for (size_t i = 0; i < lines.size(); ++i)
            std::cout << i + 1 << ": " << lines[i] << "\n";
        }
        continue;
      }

      if (input == "help") {
        PrintHelp();
        continue;
      }

      lines.push_back(input);
    }
  }

 private:
  void RunAssembler(const std::vector<std::string>& lines) {
    try {
      Assembler assembler;
      assembler.ReadSourceLines(lines).ThrowIfError();
      assembler.AssembleProgram().ThrowIfError();
      assembler.WriteBinaryOutput("../spu/out.spu").ThrowIfError();
      std::cout << "Assembled successfully → out.spu\n";
    } catch (const AssemblerException& e) {
      std::cout << "Error: " << e.what() << "\n";
    }
  }

  static void PrintHelp() {
    std::cout << "Assembler interactive mode\n";
    std::cout << "Commands:\n";
    std::cout << "  run   — assemble and write to out.spu\n";
    std::cout << "  list  — show current instructions\n";
    std::cout << "  clear — clear all instructions\n";
    std::cout << "  help  — show this message\n";
    std::cout << "  quit  — exit\n\n";
  }
};

}