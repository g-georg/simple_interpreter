#pragma once

#include <array>
#include <stddef.h>
#include <stdint.h>
#include <string_view>

namespace spu {

constexpr size_t kVideoWidth = 50;
constexpr size_t kVideoHeight = 20;
constexpr size_t kMaxRamSize = kVideoWidth * kVideoHeight;

constexpr size_t kRegisterCount = 8;

constexpr std::array<std::string_view, kRegisterCount> kRegisterNames = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

constexpr size_t kMaxLabelLen = 63;
constexpr size_t kMaxLabelsCount = 10;

constexpr uint32_t kAsmVersion = 1338;

}  // namespace spu