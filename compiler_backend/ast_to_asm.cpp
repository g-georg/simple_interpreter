#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <memory>
#include "AstParser.hpp"
#include "AsmGenerator.hpp"
#include "readFile.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ast> [output.asm]\n";
        return 1;
    }

    const std::string inputFile  = argv[1];
    const std::string outputFile = (argc >= 3) ? argv[2] : "output.asm";

    std::string source;
    try {
        source = readFile(inputFile);
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    NamesTable names;
    Node* root = nullptr;

    try {
        AstParser parser(source, names);
        root = parser.parse();
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Parse error: " << e.what() << "\n";
        return 1;
    }

    std::ofstream outStream(outputFile);
    if (!outStream.is_open())  {
        std::cerr << "[ERROR] Cannot open output file: " << outputFile << "\n";
        delete root;
        return 1;
    }

    try {
        AsmGenerator gen(names, outStream);
        gen.generate(root);
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Codegen error: " << e.what() << "\n";
        delete root;
        return 1;
    }

    delete root;

    std::cout << "Compiled \"" << inputFile << "\" -> \"" << outputFile << "\"\n";
    return 0;
}
