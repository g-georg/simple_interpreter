#include "Parser.h"

NodePtr Parser::parse(std::vector<Token> tokens, NameTable& names) {
    tokens_ = std::move(tokens);
    names_  = &names;
    cur_    = 0;
    return parseGramma();
}

NodePtr Parser::parseGramma() {
    NodePtr root = tryParseTopLevel();
    if (!root)
        syntaxError("Program must start with main or func");
    root = CONNECT_(std::move(root), nullptr);

    while (!atEnd()) {
        NodePtr next = tryParseTopLevel();
        if (!next)
            syntaxError("Expected function or main declaration");

        root = CONNECT_(std::move(root), std::move(next));
    }
    return root;
}

NodePtr Parser::tryParseTopLevel() {
    if (isKeyword(Keyword::Func))
        return parseFunction();
    if (isKeyword(Keyword::Main))
        return parseMain();
    return nullptr;
}

NodePtr Parser::parseFunction() {
    expect(Keyword::Func, "excepted `func`");

    NodePtr node = FUNC_(nullptr, nullptr);
    node->left = COMMA_(nullptr, nullptr);
    node->left->left = parseFuncName();

    parseFuncCommon(node);
    return node;
}

NodePtr Parser::parseMain() {
    expect(Keyword::Main, "expected `main`");
    NodePtr node = MAIN_(nullptr, nullptr);
    node->left = parseFuncName();

    parseFuncCommon(node);
    return node;
}

void Parser::parseFuncCommon(NodePtr& funcNode) {
    expect(Keyword::OpenParens, "expected `(` after func name");
    expect(Keyword::CloseParens, "Expected ')' after '('");
    expect(Keyword::OpenBracket, "Expected '{' ");

    while (!isKeyword(Keyword::Return)) {
        NodePtr op = parseOperation();
        funcNode->right = CONNECT_(std::move(funcNode->right), std::move(op));
    }

    funcNode->right = CONNECT_(std::move(funcNode->right), nullptr);
    NodePtr& body = funcNode->right;
    body->right = parseReturn();

    expect(Keyword::CloseBracket, "Expected '}' ");
}

NodePtr Parser::parseFuncName() {
    if (!isType(NodeType::Name))
        syntaxError("Expected function name");
    
    size_t idx = peek().value.idx;
    if (declaredFuncs_.count(idx))
        syntaxError("Redeclaration of function '" + names_->findByIdx(idx)->name + "'");

    declaredFuncs_.insert(idx);
    declaredVars_.insert(idx);

    advance();
    return NAME(idx);
}

NodePtr Parser::parseOperation() {
    if (isKeyword(Keyword::If))
        return parseIf();
    if (isKeyword(Keyword::OpenBracket))
        return parseBlock();

    if (auto n = tryDeclarateOrAssign())
        return n;
    
    if (auto n = tryFunctionCall()) {
        if (isKeyword(Keyword::Connect))
            advance();
        return n;
    }

    if (auto n = tryPrint())
        return n;

    syntaxError("Unknown operation");
}

NodePtr Parser::parseIf() {
    expect(Keyword::If, "Expected 'if'");
    expect(Keyword::OpenParens, "Expected '(' after 'if'");

    NodePtr cond = parseExpression();

    expect(Keyword::CloseParens, "Expected ')' after if-condition");
    NodePtr body = parseOperation();

    return IF_(std::move(cond), std::move(body));
}

NodePtr Parser::parseBlock() {
    advance();
    NodePtr first = parseOperation();
    while (!isKeyword(Keyword::CloseBracket)) {
        NodePtr next = parseOperation();
        first = CONNECT_(std::move(first), std::move(next));
    }
    advance();
    return first;
}

NodePtr Parser::tryDeclarateOrAssign() {
    if (!isType(NodeType::Name))
        return nullptr;

    bool isDecl = isNextKeyword(Keyword::Declarate);
    bool isAssign = isNextKeyword(Keyword::Assign);
    if (!isDecl && !isAssign)
        return nullptr;
    size_t idx = peek().value.idx;
    advance();

    Keyword op = static_cast<Keyword>(peek().value.idx);
    advance();
    NodePtr nameNode;
    if (isDecl) {
        if (declaredVars_.count(idx))
            syntaxError("Redeclaration of variable '" + names_->findByIdx(idx)->name + "'");

        declaredVars_.insert(idx);
        nameNode = NAME(idx);
    } else {
        if (!declaredVars_.count(idx))
            syntaxError("Assignment to undeclared variable '" + names_->findByIdx(idx)->name + "'");

        nameNode = NAME(idx);
    }

    NodePtr expr = parseExpression();
    expect(Keyword::Connect, "Expected ';' after expression");
    return KW(op, std::move(nameNode), std::move(expr));
}

NodePtr Parser::tryPrint() {
    if (!isKeyword(Keyword::Print))
        return nullptr;
    advance();

    expect(Keyword::OpenParens, "Expected '(' after 'print'");
    NodePtr expr = parseExpression();
    expect(Keyword::CloseParens, "Expected ')' after print argument");
    expect(Keyword::Connect, "Expected ';' after expression");
    return CONNECT_(PRINT_(std::move(expr), nullptr), nullptr);
}

NodePtr Parser::parseReturn() {
    expect(Keyword::Return, "Expected 'return'");
    NodePtr expr = parseExpression();
    if (isKeyword(Keyword::Connect))
        advance();
    return CONNECT_(RETURN_(std::move(expr), nullptr), nullptr);
}

NodePtr Parser::tryReturn() {
    if (!isKeyword(Keyword::Return))
        return nullptr;
    return parseReturn();
}

NodePtr Parser::parseExpression() {
    NodePtr node = parseTerm();
    while (isKeyword(Keyword::Add) || isKeyword(Keyword::Sub)) {
        Keyword op = static_cast<Keyword>(peek().value.idx);
        advance();
        NodePtr rhs = parseTerm();
        if (op == Keyword::Add)
            node = ADD_(std::move(node), std::move(rhs));
        else
            node = SUB_(std::move(node), std::move(rhs));
    }
    return node;
}

NodePtr Parser::parseTerm() {
    NodePtr node = parsePower();
    while (isKeyword(Keyword::Mul) || isKeyword(Keyword::Div)) {
        Keyword op = static_cast<Keyword>(peek().value.idx);
        advance();
        NodePtr rhs = parsePower();

        if (op == Keyword::Mul)
            node = MUL_(std::move(node), std::move(rhs));
        else
            node = DIV_(std::move(node), std::move(rhs));
    }
    return node;
}

NodePtr Parser::parsePower() {
    NodePtr node = parsePrimary();

    while (isKeyword(Keyword::Pow)) {
        advance();
        NodePtr rhs = parsePrimary();
        node = POW_(std::move(node), std::move(rhs));
    }
    return node;
}

NodePtr Parser::parsePrimary() {
    if (isKeyword(Keyword::OpenParens)) {
        advance();
        NodePtr node = parseExpression();
        expect(Keyword::CloseParens, "Expected ')' after expression");
        return node;
    }

    if (isType(NodeType::ConstNum)) {
        ValueNumber n = peek().value.number;
        advance();
        return NUM(n);
    }

    if (auto node = tryBuitlinFunction())
        return node;
    if (auto node = tryFunctionCall())
        return node;
    if (auto node = tryVariable())
        return node;
    
    syntaxError("Expected expression");
}   

NodePtr Parser::tryBuitlinFunction() {
    if (isType(NodeType::Keyword))
        return nullptr;

    Keyword kw = static_cast<Keyword>(peek().value.idx);
    const KeywordInfo* info = FindBuiltinFunction(kw);
    if (!info)
        return nullptr;

    advance();
    expect(Keyword::OpenParens, "Expected '(' after function name");
    NodePtr node = KW(kw);
    if (info->numberOfArgs == 0) {
        expect(Keyword::CloseParens, "Expected ')' after function arguments");
        return node;
    }

    NodePtr first = parseExpression();

    if (info->numberOfArgs == 1) {
        expect(Keyword::CloseParens, "Expected ')' after function arguments");
        node->left = std::move(first);
        return node;
    }
    expect(Keyword::Comma, "Expected ',' between function arguments");
    NodePtr second = parseExpression();
    expect(Keyword::CloseParens, "Expected ')' after function arguments");
    node->left = std::move(first);
    node->right = std::move(second);
    return node;
}

NodePtr Parser::tryFunctionCall() {
    if (!isKeyword(Keyword::Call))
        return nullptr;
    advance();
    if (!isType(NodeType::Name))
        syntaxError("Expected function name after 'call'");

    size_t idx = peek().value.idx;
    if (!declaredFuncs_.count(idx) && !declaredVars_.count(idx))
        syntaxError("Call to undeclared function '" + names_->findByIdx(idx)->name + "'");
    advance();

    expect(Keyword::OpenParens, "Expected '(' after function name in call");
    expect(Keyword::CloseParens, "Expected ')' to close function call");

    return CALL_(NAME(idx), nullptr);
}

NodePtr Parser::tryVariable() {
    if (!isType(NodeType::Name))
        return nullptr;

    size_t idx = peek().value.idx;

    if (!declaredVars_.count(idx))
        syntaxError("Variable '" + names_->findByIdx(idx)->name + "' used before declaration");

    advance();
    return NAME(idx);
}