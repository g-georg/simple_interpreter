#include "AsmGenerator.hpp"
#include <cassert>
#include <iostream>

void AsmGenerator::generate(Node* root) {
    out_ << "; Compiled from rap language (ast_to_asm)\n\n";
    out_ << "CALL :main\n";
    out_ << "HLT\n";
    assembleNode(root);
}

const std::string& AsmGenerator::getName(size_t idx) const {
    return names_.get(idx);
}

void AsmGenerator::assembleNode(Node* node) {
    if (node == nullptr) {
        throw std::runtime_error("assembleNode: got nullptr");
    }

    switch (node->type) {
        case TYPE_UNKNOWN: {
            throw std::runtime_error("assembleNode: TYPE_UNKNOWN node");
        }

        case TYPE_CONST_NUM: {
            out_ << "PUSH " << node->number << "\n";
            break;
        }

        case TYPE_VARIABLE: {
            out_ << "PUSH "   << node->idx << "\n"
                 << "POPR RAX\n"
                 << "PUSHM [RAX]\n\n";
            break;
        }

        case TYPE_NAME: {
            throw std::runtime_error(
                "assembleNode: TYPE_NAME node should not be assembled directly");
        }

        case TYPE_KEYWORD: {
            assembleKeyword(node);
            break;
        }

        default: {
            throw std::runtime_error("assembleNode: unknown node type");
        }
    }
}

void AsmGenerator::assembleKeyword(Node* node) {
    assert(node->type == TYPE_KEYWORD);
    auto kwIdx = static_cast<KeywordIdx>(node->idx);
    const KeywordInfo* kw = FindKeywordByIdx(kwIdx);

    if (kw && kw->numberOfArgs >= 1 && node->left) {
        assembleNode(node->left);
    }
    if (kw && kw->numberOfArgs == 2 && node->right) {
        assembleNode(node->right);
    }

    switch (kwIdx) {
        case KEY_ADD: {
            out_ << "ADD\n\n";
            break;
        }

        case KEY_SUB: {
            out_ << "SUB\n\n";
            break;
        }

        case KEY_MUL: {
            out_ << "MUL\n\n";
            break;
        }

        case KEY_DIV: {
            out_ << "DIV\n\n";
            break;
        }

        case KEY_POW: {
            out_ << "; POW (not implemented in VM, stub)\n"
                 << "CALL :__pow\n\n";
            break;
        }

        case KEY_LOG:
        case KEY_LN:
        case KEY_SIN:
        case KEY_COS:
        case KEY_TG:
        case KEY_CTG:
        case KEY_ARCSIN:
        case KEY_ARCCOS:
        case KEY_ARCTG:
        case KEY_ARCCTG:
        case KEY_SH:
        case KEY_CH:
        case KEY_TH:
        case KEY_CTH: {
            const char* mathName = kw ? kw->asmName : "math_fn";
            out_ << "; Math function: " << mathName << "\n"
                 << "CALL :__" << mathName << "\n\n";
            break;
        }

        case KEY_INPUT: {
            out_ << "IN\n";
            break;
        }

        case KEY_PRINT: {
            out_ << "OUT\n";
            break;
        }

        case KEY_CONNECT: {
            if (node->left) {
                assembleNode(node->left);
            }
            if (node->right) {
                assembleNode(node->right);
            }
            break;
        }

        case KEY_IF: {
            assembleIf(node);
            break;
        }

        case KEY_DECLARATE:
        case KEY_ASSIGN: {
            if (!node->right) {
                throw std::runtime_error("ASSIGN/DECLARATE: missing right child");
            }
            assembleNode(node->right);

            if (!node->left || node->left->type != TYPE_VARIABLE) {
                throw std::runtime_error("ASSIGN/DECLARATE: left child must be TYPE_VARIABLE");
            }

            size_t varIdx = node->left->idx;
            out_ << "PUSH "   << varIdx << "\n"
                 << "POPR RAX\n"
                 << "POPM [RAX]\n\n";
            break;
        }

        case KEY_FUNC: {
            assembleFunc(node);
            break;
        }

        case KEY_MAIN: {
            assembleMain(node);
            break;
        }

        case KEY_RETURN: {
            if (node->left) {
                assembleNode(node->left);
            } else {
                out_ << "; return без значения\n";
            }

            out_ << "POPR RAX\n"
                 << "RET\n\n";
            break;
        }

        case KEY_CALL: {
            assembleCall(node);
            break;
        }

        case KEY_COMMA: {
            std::cerr << "[WARNING] KEY_COMMA in AST – ignoring\n";
            break;
        }

        case KEY_UNKNOWN: {
            throw std::runtime_error("assembleKeyword: KEY_UNKNOWN");
        }

        default: {
            throw std::runtime_error(
                std::string("assembleKeyword: unhandled keyword idx=") +
                std::to_string(kwIdx));
        }
    }
}

void AsmGenerator::assembleIf(Node* node) {
    size_t label = ifCounter_++;

    out_ << "; if #" << label << "\n";

    if (!node->left) {
        throw std::runtime_error("IF: missing condition (left child)");
    }
    assembleNode(node->left);

    out_ << "PUSH 0\n"
         << "JE :endif_" << label << "\n";

    if (!node->right) {
        throw std::runtime_error("IF: missing body (right child)");
    }
    assembleNode(node->right);

    out_ << ":endif_" << label << "\n\n";
}

void AsmGenerator::assembleFunc(Node* node) {

    Node* args = node->left;
    if (!args) {
        throw std::runtime_error("FUNC: missing arguments node");
    }

    Node* funcNameNode = args->left;
    if (!funcNameNode) {
        throw std::runtime_error("FUNC: missing function name node");
    }

    const std::string& funcName = getName(funcNameNode->idx);
    out_ << "\n:" << funcName << "\n";

    if (args->right) {
        assembleParams(args->right);
    }

    Node* body = node->right;
    if (!body) {
        throw std::runtime_error("FUNC: missing body");
    }
    assembleNode(body);
}

void AsmGenerator::assembleParams(Node* node) {
    if (!node) {
        return;
    }
    if (node->type == TYPE_VARIABLE) {
        out_ << "; param " << getName(node->idx) << "\n"
             << "PUSH "   << node->idx << "\n"
             << "POPR RAX\n"
             << "POPM [RAX]\n";
        return;
    }
    if (node->type == TYPE_KEYWORD &&
        static_cast<KeywordIdx>(node->idx) == KEY_COMMA) {
        assembleParams(node->left);
        assembleParams(node->right);
        return;
    }
    assembleParams(node->left);
    assembleParams(node->right);
}

void AsmGenerator::assembleMain(Node* node) {
    out_ << "\n:main\n";

    Node* body = node->right;
    if (!body) {
        throw std::runtime_error("MAIN: missing body");
    }
    assembleNode(body);
}

void AsmGenerator::assembleCall(Node* node) {
    Node* funcNameNode = node->left;
    if (!funcNameNode) {
        throw std::runtime_error("CALL: missing function name");
    }

    const std::string& funcName = getName(funcNameNode->idx);

    if (node->right) {
        assembleNode(node->right);
    }

    out_ << "\nCALL :" << funcName << "\n"
         << "PUSHR RAX\n\n";
}