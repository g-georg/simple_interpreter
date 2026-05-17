#ifndef COMPILER_ASTPARSER_HPP
#define COMPILER_ASTPARSER_HPP
#include <string>
#include "Node.hpp"
#include "NamesTable.hpp"
#include <iostream>

class AstParser {
public:
    AstParser(const std::string& source, NamesTable& names)
        : src_(source), pos_(0), names_(names) {}

    Node* parse();

private:
    const std::string& src_;
    size_t             pos_;
    NamesTable&        names_;


    void skipSpaces();
    char peek() const;
    char consume();
    bool tryConsume(const std::string& s);
    std::string readToken();
    Node* parseNode();
    Node* parseNodeContents();
};


#endif