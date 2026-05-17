#include "ast_frontend.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "AstPrinter.h"
#include <iostream>
#include <memory>

bool buildAST(const std::string& sourceFile, 
              NodePtr& outAst, 
              std::unique_ptr<NameTable>& outNames) {
    try {
        auto names = std::make_unique<NameTable>();
        Tokenizer tokenizer;

        auto tokens = tokenizer.tokenizeFile(sourceFile, *names);

        Parser parser;
        NodePtr ast = parser.parse(std::move(tokens), *names);

        outAst = std::move(ast);
        outNames = std::move(names);

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "[FRONTEND ERROR] " << e.what() << '\n';
        return false;
    }
}
