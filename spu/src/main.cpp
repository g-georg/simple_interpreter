#include <iostream>

#include "common/error_codes.h"
#include "core/spu.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " file.spu\n";
    return 1;
  }

  spu::Spu spu;
  spu::SpuError load_status = spu.LoadBytecode(argv[1]);
  if (load_status != spu::SpuError::kOk) {
    spu::PrintSpuError(load_status);
    return static_cast<int>(spu::ToUnderlying(load_status));
  }

  spu::RuntimeError run_status = spu.Run();
  if (run_status != spu::RuntimeError::kOk) {
    spu::PrintRuntimeError(run_status);
    return static_cast<int>(spu::ToUnderlying(run_status));
  }

  return 0;
}