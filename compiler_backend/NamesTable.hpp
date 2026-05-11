#include <string>
#include <unordered_map>
#include <vector>
class NamesTable {
public:
    size_t findOrAdd(const std::string& name);
    const std::string& get(size_t idx) const;
    size_t size() const;

private:
    std::vector<std::string> names_;
    std::unordered_map<std::string, size_t> index_;
};