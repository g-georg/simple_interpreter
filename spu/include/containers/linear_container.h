#pragma once

#include "container.h"

namespace spu {

template<typename T>
class LinearContainer : public Container {
 public:
  using value_type = T;

  virtual void Clear() = 0;
};

}
