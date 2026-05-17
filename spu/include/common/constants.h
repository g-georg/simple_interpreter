#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace spu {

constexpr std::size_t kVideoWidth = 50;
constexpr std::size_t kVideoHeight = 20;
constexpr std::size_t kMaxRamSize = kVideoWidth * kVideoHeight;

constexpr std::size_t kRegisterCount = 8;
constexpr std::array<std::string_view, kRegisterCount> kRegisterNames = {
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

constexpr std::size_t kMaxLabelLen = 63;
constexpr std::size_t kMaxLabelsCount = 10;

constexpr std::uint32_t kAsmVersion = 1338;

}  // namespace spu
