#pragma once

#include <vector>
#include <stdexcept>

#include "linear_container.h"

namespace spu {

template<typename T>
concept StackValue = std::is_trivially_copyable_v<T>;

template<StackValue T>
class Stack final : public LinearContainer<T> {
 public:
  void Push(const T& value) {
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
    return data_.back();
  }

  [[nodiscard]] std::size_t Size() const override {
    return data_.size();
  }

  [[nodiscard]]
  bool Empty() const override {
    return data_.empty();
  }

  void Clear() override {
    data_.clear();
  }

 private:
  std::vector<T> data_;
};

}
