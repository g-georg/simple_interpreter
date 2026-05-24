#pragma once

#include <concepts>

#include "commands/command_base.h"

namespace spu {

template <typename T>
concept Command = std::derived_from<T, ICommand>;

}  // namespace spu