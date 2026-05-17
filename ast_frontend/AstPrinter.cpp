#include "AstPrinter.h"

void  AstPrinter::saveToFile(const std::string& fileName, const AstNode& root, const NameTable& names) {
    std::ofstream f(fileName);
    if (!f)
        throw std::runtime_error("Cannot open file for writing: " + fileName);

    printNode(f, root, names, 0);
    f << '\n';
}

void AstPrinter::indent(std::ofstream& f, int depth) {
    for (int i = 0; i < depth; ++i)
        f << '\t';
}

void AstPrinter::printNodeValue(std::ofstream& f,
                    const AstNode& n,
                    const NameTable& names) {
    switch (n.type) {
        case NodeType::Unknown:  f << "UNKNOWN"; break;
        case NodeType::ConstNum: f << n.value.number; break;
        case NodeType::Keyword: {
            const auto* ki = FindKeywordInfo(n.keyword());
            f << (ki ? ki->standardName : "?");
            break;
        }
        case NodeType::Name: {
            const auto* e = names.findByIdx(n.value.idx);
            if (e) f << '"' << e->name << '"';
            else   f << "\"?\"";
            break;
        }
    }
}

void AstPrinter::printNode(std::ofstream& f,
                const AstNode& n,
                const NameTable& names,
                int depth) {
    f << "( ";
    printNodeValue(f, n, names);
    f << '\n';

    indent(f, depth + 1);
    if (n.left)  printNode(f, *n.left,  names, depth + 1);
    else         f << "nil";

    f << '\n';
    indent(f, depth + 1);
    if (n.right) printNode(f, *n.right, names, depth + 1);
    else         f << "nil";

    f << '\n';
    indent(f, depth);
    f << ')';
}