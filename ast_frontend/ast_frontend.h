#pragma once

#include <string>
#include <memory>

struct AstNode;
class NameTable;

using NodePtr = std::unique_ptr<AstNode>;

bool buildAST(const std::string& sourceFile, 
              NodePtr& outAst, 
              std::unique_ptr<NameTable>& outNames);
