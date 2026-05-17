#pragma once

#include <stddef.h>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "containers/linear_container.h"

namespace spu {

template <typename T>
concept StackValue = std::is_trivially_copyable_v<T>;

template <StackValue T>
class Stack final : public LinearContainer<T> {
 public:
  using value_type = T;

  Stack() = default;

  explicit Stack(size_t reserve_size) {
    data_.reserve(reserve_size);
  }

  void Push(T value) {
    data_.push_back(value);
  }

  T Pop() {
    if (data_.empty()) {
      throw std::underflow_error("stack underflow");
    }

    T value = data_.back();
    data_.pop_back();

    return value;
  }

  [[nodiscard]] const T& Top() const {
    if (data_.empty()) {
      throw std::underflow_error("stack underflow");
    }

    return data_.back();
  }

  [[nodiscard]] size_t Size() const override {
    return data_.size();
  }

  [[nodiscard]] bool Empty() const override {
    return data_.empty();
  }

  void Clear() override {
    data_.clear();
  }

 private:
  std::vector<T> data_;
};

}  // namespace spu