#pragma once

#include "containers/container.h"

namespace spu {

template <typename T>
class LinearContainer : public Container {
 public:
  using value_type = T;

  ~LinearContainer() override = default;

  virtual void Clear() = 0;
};

}  // namespace spu