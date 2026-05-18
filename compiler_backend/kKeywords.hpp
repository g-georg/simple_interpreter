#pragma once
#include <cstddef>
#include <string>
#include "KeywordInfo.hpp"

enum NodeType {
    TYPE_UNKNOWN  = 0,
    TYPE_CONST_NUM,
    TYPE_KEYWORD,
    TYPE_VARIABLE,
    TYPE_NAME,
};

static const KeywordInfo kKeywords[] =
{
    { "uknown",  "uknown",  KEY_UNKNOWN,       false, 0 },
    { "+",       "+",       KEY_ADD,            false, 2 },
    { "-",       "-",       KEY_SUB,            false, 2 },
    { "*",       "*",       KEY_MUL,            false, 2 },
    { "/",       "/",       KEY_DIV,            false, 2 },
    { "^",       "^",       KEY_POW,            false, 2 },
    { "log",     "log",     KEY_LOG,            true,  2 },
    { "ln",      "ln",      KEY_LN,             true,  1 },
    { "sin",     "sin",     KEY_SIN,            true,  1 },
    { "cos",     "cos",     KEY_COS,            true,  1 },
    { "tg",      "tg",      KEY_TG,             true,  1 },
    { "ctg",     "ctg",     KEY_CTG,            true,  1 },
    { "arcsin",  "arcsin",  KEY_ARCSIN,         true,  1 },
    { "arccos",  "arccos",  KEY_ARCCOS,         true,  1 },
    { "arctg",   "arctg",   KEY_ARCTG,          true,  1 },
    { "arcctg",  "arcctg",  KEY_ARCCTG,         true,  1 },
    { "sh",      "sh",      KEY_SH,             true,  1 },
    { "ch",      "ch",      KEY_CH,             true,  1 },
    { "th",      "th",      KEY_TH,             true,  1 },
    { "cth",     "cth",     KEY_CTH,            true,  1 },
    { "input",   "input",   KEY_INPUT,          true,  0 },
    { "print",   "print",   KEY_PRINT,          true,  1 },
    { ";",       ";",       KEY_CONNECT,        false, 0 },
    { ",",       ",",       KEY_COMMA,          false, 0 },
    { ":=",      ":=",      KEY_DECLARATE,      false, 0 },
    { "=",       "=",       KEY_ASSIGN,         false, 0 },
    { "if",      "if",      KEY_IF,             false, 0 },
    { "(",       "(",       KEY_OPEN_PARENS,    false, 0 },
    { ")",       ")",       KEY_CLOSE_PARENS,   false, 0 },
    { "{",       "{",       KEY_OPEN_BRACKET,   false, 0 },
    { "}",       "}",       KEY_CLOSE_BRACKET,  false, 0 },
    { "func",    "func",    KEY_FUNC,           false, 0 },
    { "main",    "main",    KEY_MAIN,           false, 0 },
    { "return",  "return",  KEY_RETURN,         true,  1 },
    { "call",    "call",    KEY_CALL,           false, 0 },
};

static const size_t kNumKeywords = sizeof(kKeywords) / sizeof(KeywordInfo);

static const KeywordInfo* FindKeywordByName(const std::string& name) {
    for (size_t i = 0; i < kNumKeywords; ++i) {
        if (name == kKeywords[i].asmName) {
            return &kKeywords[i];
        }
    }
    return nullptr;
}

static const KeywordInfo* FindKeywordByIdx(KeywordIdx idx) {
    for (size_t i = 0; i < kNumKeywords; ++i) {
        if (kKeywords[i].idx == idx) {
            return &kKeywords[i];
        }
    }
    return nullptr;
}