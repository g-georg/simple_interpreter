#pragma once

#include <concepts>
#include <stddef.h>
#include <type_traits>

namespace spu {

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T>
concept Enum = std::is_enum_v<T>;

template <Enum T>
constexpr auto ToUnderlying(T value) noexcept {
  return static_cast<std::underlying_type_t<T>>(value);
}

template <typename Stack>
concept StackLike = requires(Stack stack, typename Stack::value_type value) {
  typename Stack::value_type;
  { stack.Push(value) } -> std::same_as<void>;
  { stack.Pop() } -> std::same_as<typename Stack::value_type>;
  { stack.Top() } -> std::same_as<const typename Stack::value_type&>;
  { stack.Empty() } -> std::convertible_to<bool>;
  { stack.Size() } -> std::convertible_to<size_t>;
};

}  // namespace spu