#include "backend.h"
#include "AstParser.hpp"
#include "AsmGenerator.hpp"
#include "readFile.hpp"
#include <iostream>
#include <fstream>

bool generateAssembly(const std::string& astFile, 
                      const std::string& outputFile) {
    try {
        std::string source;
        try {
            source = readFile(astFile);
        }
        catch (const std::exception& e) {
            std::cerr << "[BACKEND ERROR] Cannot read AST file: " << e.what() << "\n";
            return false;
        }

        NamesTable names;
        Node* root = nullptr;

        try {
            AstParser parser(source, names);
            root = parser.parse();
        }
        catch (const std::exception& e) {
            std::cerr << "[BACKEND ERROR] Parse error: " << e.what() << "\n";
            return false;
        }

        std::ofstream outStream(outputFile);
        if (!outStream.is_open()) {
            std::cerr << "[BACKEND ERROR] Cannot open output file: " << outputFile << "\n";
            delete root;
            return false;
        }

        try {
            AsmGenerator gen(names, outStream);
            gen.generate(root);
        }
        catch (const std::exception& e) {
            std::cerr << "[BACKEND ERROR] Assembly generation error: " << e.what() << "\n";
            delete root;
            return false;
        }

        delete root;
        outStream.close();

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[BACKEND ERROR] Unexpected error: " << e.what() << "\n";
        return false;
    }
}
