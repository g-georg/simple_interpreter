#include "assembler.hpp"

#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <source.asm>\n";
    return 1;
  }

  assembler::Assembler assembler;

  auto status = assembler.ReadSourceFile(argv[1]);
  if (!status.Ok()) {
    std::cerr << status.ToString() << "\n";
    return 1;
  }

  status = assembler.AssembleProgram();
  if (!status.Ok()) {
    std::cerr << status.ToString() << "\n";
    return 1;
  }

  status = assembler.WriteBinaryOutput("../spu/out.spu");
  if (!status.Ok()) {
    std::cerr << status.ToString() << "\n";
    return 1;
  }

  return 0;
}