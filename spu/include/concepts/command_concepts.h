#pragma once

#include <concepts>

namespace spu {

class ICommand;

template<typename T>
concept Command =
    std::derived_from<T, ICommand>;

}
