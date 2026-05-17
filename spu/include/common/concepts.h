#pragma once

#include <concepts>
#include <cstddef>
#include <stack>
#include <type_traits>

namespace spu {

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <typename Stack>
concept StackLike = requires(Stack s, typename Stack::value_type value) {
  typename Stack::value_type;
  { s.push(value) } -> std::same_as<void>;
  { s.top() } -> std::same_as<typename Stack::value_type&>;
  { s.pop() } -> std::same_as<void>;
  { s.empty() } -> std::convertible_to<bool>;
  { s.size() } -> std::convertible_to<std::size_t>;
};

template <typename U>
constexpr auto ToUnderlying(U value) noexcept {
  return static_cast<std::underlying_type_t<EU>(value);
}

}  // namespace spu
