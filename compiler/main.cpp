#include <iostream>
#include <memory>

#include "../ast_frontend/ast_frontend.h"
#include "../ast_frontend/AstPrinter.h"
#include "../ast_frontend/AstNode.hpp"
#include "../compiler_backend/backend.h"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input.jpp> [output.asm]\n";
        return 1;
    }

    const std::string sourceFile = argv[1];
    std::string outputFile;

    if (argc == 3) {
        outputFile = argv[2];
    } else {
        outputFile = sourceFile;
        const size_t dotPos = outputFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            outputFile = outputFile.substr(0, dotPos);
        }
        outputFile += ".asm";
    }

    const std::string astFile = "/tmp/.compiler_temp.ast";

    NodePtr ast;
    std::unique_ptr<NameTable> names;

    if (!buildAST(sourceFile, ast, names)) {
        return 1;
    }

    try {
        AstPrinter printer;
        printer.saveToFile(astFile, *ast, *names);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Failed to save AST: " << e.what() << "\n";
        return 1;
    }

    if (!generateAssembly(astFile, outputFile)) {
        return 1;
    }

    std::remove(astFile.c_str());
    return 0;
}
