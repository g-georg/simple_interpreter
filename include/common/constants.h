#ifndef INCLUDE_COMMON_CONSTANTS_H_
#define INCLUDE_COMMON_CONSTANTS_H_

#include <cstddef>
#include <cstdint>

namespace spu {

constexpr int32_t kVideoWidth = 50;
constexpr int32_t kVideoHeight = 20;
constexpr size_t kMaxRamSize = static_cast<size_t>(kVideoWidth * kVideoHeight);

constexpr size_t kRegisterCount = 8;
constexpr size_t kRegisterNameLen = 3;
constexpr size_t kShiftRegister = 5;

constexpr int32_t kMaxOpcodeValue = 42;
constexpr size_t kMaxLabelLen = 63;
constexpr size_t kMaxLabelsCount = 10;

constexpr uint32_t kAsmVersion = 1338;

}

#endif