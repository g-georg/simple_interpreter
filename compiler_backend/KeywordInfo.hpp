#include "KeywordIdx.hpp"
struct KeywordInfo
{
    const char* asmName;
    const char* stdName;
    KeywordIdx  idx;
    bool isFunction;
    int numberOfArgs;
};