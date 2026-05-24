#pragma once

#include <stddef.h>

namespace spu {

class Container {
 public:
  virtual ~Container() = default;

  [[nodiscard]] virtual size_t Size() const = 0;
  [[nodiscard]] virtual bool Empty() const = 0;
};

}  // namespace spu