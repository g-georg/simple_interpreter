#include "NamesTable.hpp"

size_t NamesTable::findOrAdd(const std::string& name) {
    auto it = index_.find(name);
    if (it != index_.end())
        return it->second;
    size_t idx = names_.size();
    names_.push_back(name);
    index_[name] = idx;
    return idx;
}

const std::string& NamesTable::get(size_t idx) const {
    if (idx >= names_.size()) {
        throw std::runtime_error("NamesTable: index out of range: " + std::to_string(idx));
    }
    return names_[idx];
}

size_t NamesTable::size() const {
    return names_.size();
}