
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctype.h>
#include <string.h>

#include "Token.hpp"

struct NameEntry {
    std::string name;
    size_t idx = 0;
};

class NameTable {
 public:
    size_t findOrAdd(const char *str, size_t len);

    const NameEntry *findByIdx(size_t idx) const;

    const NameEntry *findByName(const char *str, size_t len) const;

 private:
    std::vector<NameEntry> entries_;
};


class SyntaxError : public std::runtime_error {
 public:
    explicit SyntaxError(const std::string &msg) : std::runtime_error(msg) {}
};

class Tokenizer {
 public:
    std::vector<Token> tokenizeFile(const std::string& fileName, NameTable& names);

 private:

    std::vector<Token> tokenize(const std::string& source, NameTable& names);
    
    std::string source_;
};