#include "assembler.hpp"
#include "debug.hpp"

#include <cstdio>

namespace {
constexpr char kDefaultOutputPath[] = "../spu/out.spu";
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <source.asm>\n", argv[0]);
    return 1;
  }

  assembler::Assembler assembler;

  assembler::AssemblerErrorHandler status = assembler.ReadSourceFile(argv[1]);
  if (!status.Ok()) {
    fprintf(stderr, "Failed to read source file \"%s\"\n", argv[1]);
    return 1;
  }

  status = assembler.AssembleProgram();
  if (!status.Ok()) {
    fprintf(stderr, "Assembly failed\n");
    status.ToString();
    return 1;
  }

  status = assembler.WriteBinaryOutput(kDefaultOutputPath);
  if (!status.Ok()) {
    fprintf(stderr, "Failed to write output to \"%s\"\n", kDefaultOutputPath);
    status.ToString();
    return 1;
  }

  return 0;
}