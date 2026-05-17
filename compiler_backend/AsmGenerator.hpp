#pragma once
#include "Node.hpp"
#include "NamesTable.hpp"
#include <fstream>

class AsmGenerator {
public:
    AsmGenerator(const NamesTable& names, std::ostream& out)
        : names_(names), out_(out), ifCounter_(0) {}

    void generate(Node* root);

private:
    const NamesTable& names_;
    std::ostream&     out_;
    size_t            ifCounter_;

    const std::string& getName(size_t idx) const;
    void assembleNode(Node* node);
    void assembleKeyword(Node* node);
    void assembleIf(Node* node);
    void assembleFunc(Node* node);
    void assembleParams(Node* node);
    void assembleMain(Node* node);
    void assembleCall(Node* node);
};