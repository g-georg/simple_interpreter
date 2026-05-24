#include "assembler.hpp"

#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <source.asm>\n";
    return 1;
  }

  try {
    assembler::Assembler assembler;
    assembler.ReadSourceFile(argv[1]).ThrowIfError();
    assembler.AssembleProgram().ThrowIfError();
    assembler.WriteBinaryOutput("../spu/out.spu").ThrowIfError();
  } catch (const assembler::UnknownCommandException& e) {
    std::cerr << "[" << e.ErrorType() << "] " << e.what() << "\n";
    return 1;
  } catch (const assembler::InvalidLabelException& e) {
    std::cerr << "[" << e.ErrorType() << "] " << e.what() << "\n";
    return 1;
  } catch (const assembler::AssemblerException& e) {
    std::cerr << "[" << e.ErrorType() << "] " << e.what() << "\n";
    return 1;
  }

  return 0;
}