#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "assembler_errors.hpp"
#include "assembler_types.hpp"
#include "command_table.hpp"
#include "file.h"
#include "spu_common.h"
#include <cstddef>
#include <cstdio>

constexpr int UNINITIALIZED_LABEL_VALUE = -1;

struct Label
{
    unsigned long hash         = 0;
    ssize_t       addressValue = 0;
    char         *name         = nullptr;
};

struct Assembler
{
    text_t  sourceText           = {};
    int    *bytecode             = nullptr;
    size_t  instructionPointer   = 0;
    size_t  instructionsCapacity = 0;
    char   *sourceFileName       = nullptr;
    size_t  currentLineNumber    = 0;
    Label   labels[SPU_MAX_LABELS_COUNT] = {};
    size_t  labelsCount          = 0;
    FILE   *listingFile          = nullptr;
};

int AllocateBytecode    (Assembler *assembler);
int InitializeAssembler (Assembler *assembler);
int ReadSourceFile      (Assembler *assembler, char *inputFileName);
int AssembleProgram     (Assembler *assembler);
int WriteBinaryOutput   (const char *outputFileName, Assembler *assembler);
int PrintAssemblerError (int error);
int DestroyAssembler    (Assembler *assembler);

#endif