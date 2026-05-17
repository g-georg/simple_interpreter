#ifndef COMPILER_READFILE_HPP
#define COMPILER_READFILE_HPP
#include <stdexcept>

static std::string readFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
#endif