#pragma once

#include <fstream>
#include <string>
#include <stdexcept>

#include "AstNode.hpp"
#include "Tokenizer.h" 

class AstPrinter {
 public:
    void saveToFile(const std::string& fileName,
                    const AstNode&     root,
                    const NameTable&  names);
 private:
    void indent(std::ofstream& f, int depth);

    void printNodeValue(std::ofstream& f,
                        const AstNode& n,
                        const NameTable& names);

    void printNode(std::ofstream& f,
                   const AstNode& n,
                   const NameTable& names,
                   int depth);
};
