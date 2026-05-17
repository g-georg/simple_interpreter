#pragma once

#include <cstddef>

namespace spu {

class Container {
 public:
  virtual ~Container() = default;

  [[nodiscard]] virtual std::size_t Size() const = 0;

  [[nodiscard]] virtual bool Empty() const = 0;
};

}
