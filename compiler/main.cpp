#include <iostream>
#include <memory>
#include <cstring>

#include "../ast_frontend/ast_frontend.h"
#include "../ast_frontend/AstPrinter.h"
#include "../ast_frontend/AstNode.hpp"

#include "../compiler_backend/backend.h"

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [options] <input.jpp> [output.asm]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --ast-only          Generate only AST file, no assembly\n";
    std::cout << "  --keep-ast          Keep intermediate AST file after compilation\n";
    std::cout << "  -h, --help          Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << progName << " program.jpp              # Generate program.asm\n";
    std::cout << "  " << progName << " program.jpp out.asm     # Generate out.asm\n";
    std::cout << "  " << progName << " --ast-only program.jpp # Generate only AST file\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    bool astOnly = false;
    bool keepAst = false;
    std::string sourceFile;
    std::string outputFile;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--ast-only") == 0) {
            astOnly = true;
        }
        else if (std::strcmp(argv[i], "--keep-ast") == 0) {
            keepAst = true;
        }
        else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        }
        else if (argv[i][0] != '-') {
            if (sourceFile.empty()) {
                sourceFile = argv[i];
            }
            else if (outputFile.empty()) {
                outputFile = argv[i];
            }
        }
    }

    if (sourceFile.empty()) {
        std::cerr << "[ERROR] Input file not specified\n";
        printUsage(argv[0]);
        return 1;
    }

    if (outputFile.empty()) {
        outputFile = sourceFile;
        size_t dotPos = outputFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            outputFile = outputFile.substr(0, dotPos);
        }
        outputFile += ".asm";
    }

    std::string astFile = "/tmp/.compiler_temp.ast";

    std::cout << "[FRONTEND] Parsing: " << sourceFile << "\n";

    NodePtr ast;
    std::unique_ptr<NameTable> names;

    if (!buildAST(sourceFile, ast, names)) {
        std::cerr << "[COMPILER] Frontend failed. Aborting.\n";
        return 1;
    }

    std::cout << "[FRONTEND] AST built successfully\n";

    if (astOnly) {
        std::cout << "[COMPILER] --ast-only flag set. Stopping after frontend.\n";
        return 0;
    }

    std::cout << "[COMPILER] Saving AST to temporary file: " << astFile << "\n";

    try {
        AstPrinter printer;
        printer.saveToFile(astFile, *ast, *names);
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save AST: " << e.what() << "\n";
        return 1;
    }

    std::cout << "[BACKEND] Generating assembly from AST\n";

    if (!generateAssembly(astFile, outputFile)) {
        std::cerr << "[COMPILER] Backend failed. Aborting.\n";
        return 1;
    }

    if (!keepAst) {
        std::remove(astFile.c_str());
    }
    else {
        std::cout << "[INFO] Keeping temporary AST file: " << astFile << "\n";
    }

    std::cout << "[SUCCESS] Compilation completed!\n";
    std::cout << "  Source:    " << sourceFile << "\n";
    std::cout << "  Output:    " << outputFile << "\n";

    return 0;
}
