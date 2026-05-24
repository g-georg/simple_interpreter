#include "Tokenizer.h"

size_t NameTable::findOrAdd(const char *str, size_t len) {
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].name.size() == len && entries_[i].name.compare(0, len, str, len) == 0)
            return entries_[i].idx;
    }
    
    size_t idx = entries_.size();
    entries_.push_back({std::string(str, len), idx});
    return idx;
}

const NameEntry* NameTable::findByIdx(size_t idx) const {
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].idx == idx)
            return &entries_[i];
    }
    return nullptr;
}

const NameEntry* NameTable::findByName(const char *str, size_t len) const{
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].name.size() == len && entries_[i].name.compare(0, len, str, len) == 0)
            return &entries_[i];
    }
    return nullptr;
}


std::vector<Token> Tokenizer::tokenizeFile(const std::string& fileName, NameTable& names) {
    std::ifstream f(fileName.c_str());
    if (!f)
        throw std::runtime_error("can`t open " + fileName);

    std::ostringstream ss;
    ss << f.rdbuf();
    source_ = ss.str();
    return tokenize(source_, names);
}


std::vector<Token> Tokenizer::tokenize(const std::string& source, NameTable& names) {
    std::vector<Token> tokens;
    const char *p = source.c_str();
    const char *end = p + source.size();

    size_t line = 1;
    size_t col = 1;
    
    while (p < end) {
        if (std::isspace(static_cast<unsigned char>(*p))) {
            if (*p == '\n') {
                ++line;
                col = 1;
            } else {
                ++col;
            }
            ++p;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(*p))) {
            const char *start = p;
            int num = 0;
            while (p < end && std::isdigit(static_cast<unsigned char>(*p))) {
                num = num * 10 + (*p - '0');
                ++p;
            }
            Token t;
            t.type = NodeType::ConstNum;
            t.value.number = num;
            t.line = line;
            t.col = col;
            tokens.push_back(t);
            col += static_cast<size_t>(p - start);
            continue;
        }
        
        {
            const KeywordInfo *best = nullptr;
            size_t bestLen = 0;

            for (size_t i = 0; i < kKeywordsCount; ++i) {
                const KeywordInfo& ki = kKeywords[i];
                size_t klen = strlen(ki.langName);

                if (klen <= bestLen)
                    continue;
                if (static_cast<size_t>(end - p) < klen)
                    continue;
                if (strncmp(p, ki.langName, klen) != 0)
                    continue;
                
                const char first = ki.langName[0];
                if (isalpha(static_cast<unsigned char>(first)) || first == '_') {
                    const char *after = p + klen;
                    if (after < end && (isalnum(static_cast<unsigned char>(*after)) || *after == '_'))
                        continue;
                }

                best = &ki;
                bestLen = klen;
            }

            if (best) {
                Token t;
                t.type = NodeType::Keyword;
                t.value.idx = static_cast<size_t>(best->kw);
                t.line = line;
                t.col = col;
                tokens.push_back(t);
                col += bestLen;
                p += bestLen;
                continue;
            }
        }

        if (isalpha(static_cast<unsigned char>(*p)) || *p == '_') {
            const char *start = p;
            while (p < end && (isalnum(static_cast<unsigned char>(*p)) || *p == '_'))
                p++;
            const size_t len = static_cast<size_t>(p - start);
            const size_t idx = names.findOrAdd(start, len);

            Token t;
            t.type = NodeType::Name;
            t.value.idx = idx;
            t.line = line;
            t.col = col;
            tokens.push_back(t);
            col += len;
            continue;
        }
        throw SyntaxError(
            std::string("Lexical error at line") + std::to_string(line) + ":" + std::to_string(col) + "- unknown char `" + *p + "`"
        );
    }
    return tokens;
}