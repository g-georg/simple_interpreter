#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include <stdexcept>

#include "AstNode.hpp"
#include "Tokenizer.h"

class Parser {
 public:
    NodePtr parse(std::vector<Token> tokens, NameTable& names);

 private:
    std::vector<Token> tokens_;
    NameTable *names_ = nullptr;
    size_t cur_ = 0;

    std::unordered_set<size_t> declaredVars_;
    std::unordered_set<size_t> declaredFuncs_;
    
    const Token& peek() {
        static const Token eof{};
        return cur_ < tokens_.size() ? tokens_[cur_] : eof;
    }

    bool atEnd() {
        return cur_ >= tokens_.size();
    }

    bool isKeyword(Keyword kw) {
        return !atEnd() && peek().type == NodeType::Keyword && static_cast<Keyword>(peek().value.idx) == kw;
    }

    bool isNextKeyword(Keyword kw) {
        return cur_ + 1 < tokens_.size() && tokens_[cur_ + 1].type == NodeType::Keyword && static_cast<Keyword>(tokens_[cur_ + 1].value.idx) == kw;
    }

    bool isType(NodeType t) {
        return !atEnd() && peek().type == t;
    }

    void advance() {
        if (!atEnd())
            ++cur_;
    }

    void expect(Keyword kw, const char *msg) {
        if (!isKeyword(kw))
            syntaxError(msg);
        advance();
    }

    [[noreturn]] void syntaxError(const std::string& msg) {
        std::string full = "Syntax error at line " +
                           std::to_string(peek().line) + ":" + std::to_string(peek().col) + " — " + msg;
        throw SyntaxError(full);
    }

    NodePtr parseGramma();

    NodePtr tryParseTopLevel();

    NodePtr parseFunction();

    NodePtr parseMain();
    
    void parseFuncCommon(NodePtr& funcNode);

    NodePtr parseFuncName();

    NodePtr parseOperation();

    NodePtr parseIf();
    
    NodePtr parseBlock();

    NodePtr tryDeclarateOrAssign();

    NodePtr tryPrint();

    NodePtr parseReturn();

    NodePtr tryReturn();

    NodePtr parseExpression();

    NodePtr parseTerm();

    NodePtr parsePower();

    NodePtr parsePrimary();

    NodePtr tryBuitlinFunction();

    NodePtr tryFunctionCall();

    NodePtr tryVariable();
};