#ifndef NODE_HPP
#define NODE_HPP
#include "kKeywords.hpp"
struct Node {
    NodeType type = TYPE_UNKNOWN;
    int number = 0;
    size_t idx = 0;

    Node* left  = nullptr;
    Node* right = nullptr;

    ~Node() {
        delete left;
        delete right;
    }
};
#endif