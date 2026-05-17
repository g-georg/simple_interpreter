#include "AstParser.hpp"

Node* AstParser::parse() {
    skipSpaces();
    return parseNode();
}

void AstParser::skipSpaces() {
    while (pos_ < src_.size() && std::isspace((unsigned char)src_[pos_]))
        ++pos_;
}

char AstParser::peek() const {
    return (pos_ < src_.size()) ? src_[pos_] : '\0';
}

char AstParser::consume() {
    return src_[pos_++];
}

bool AstParser::tryConsume(const std::string& s) {
    if (src_.compare(pos_, s.size(), s) == 0) {
        pos_ += s.size();
        return true;
    }
    return false;
}

std::string AstParser::readToken() {
    std::string tok;
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if (std::isspace((unsigned char)c) || c == '(' || c == ')') {
            break;
        }
        tok += c;
        ++pos_;
    }
    return tok;
}

Node* AstParser::parseNode() {
    skipSpaces();

    if (pos_ >= src_.size()) {
        throw std::runtime_error("Unexpected end of input");
    }

    if (peek() == '(') {
        consume();
        skipSpaces();

        Node* node = parseNodeContents();

        skipSpaces();
        if (peek() != ')') {
            throw std::runtime_error(
                std::string("Expected ')' at pos ") + std::to_string(pos_) +
                ", got '" + peek() + "'");
        }
        consume();
        return node;
    }

    if (src_.compare(pos_, 3, "nil") == 0) {
        pos_ += 3;
        return nullptr;
    }

    throw std::runtime_error(
        std::string("Unexpected char '") + peek() +
        "' at pos " + std::to_string(pos_));
}

Node* AstParser::parseNodeContents() {
    Node* node = new Node();

    if (peek() == '"') {
        consume();
        std::string name;
        while (pos_ < src_.size() && src_[pos_] != '"') {
            name += src_[pos_++];
        }
        if (pos_ >= src_.size()) {
            delete node;
            throw std::runtime_error("Unterminated string literal");
        }
        consume();

        size_t idx = names_.findOrAdd(name);
        node->type = TYPE_VARIABLE;
        node->idx  = idx;
    } else {
        std::string tok = readToken();
        if (tok.empty()) {
            delete node;
            throw std::runtime_error(
                std::string("Empty token at pos ") + std::to_string(pos_));
        }

        bool isNumber = false; {
            size_t i = 0;
            if (tok[i] == '-' || tok[i] == '+') {
                ++i;
            }

            if (i < tok.size()) {
                isNumber = true;
                for (; i < tok.size(); ++i) {
                    if (!std::isdigit((unsigned char)tok[i])) {
                        isNumber = false;
                        break;
                    }
                }
            }
        }

        if (isNumber) {
            node->type   = TYPE_CONST_NUM;
            node->number = std::stoi(tok);
        } else {
            const KeywordInfo* kw = FindKeywordByName(tok);
            if (kw != nullptr) {
                node->type = TYPE_KEYWORD;
                node->idx  = (size_t)kw->idx;
            } else {
                std::cerr << "[WARNING] Unknown token \"" << tok
                          << "\" at pos " << pos_ << ", treating as name\n";
                size_t idx = names_.findOrAdd(tok);
                node->type = TYPE_VARIABLE;
                node->idx  = idx;
            }
        }
    }

    skipSpaces();
    node->left = parseNode();
    skipSpaces();
    node->right = parseNode();

    return node;
}