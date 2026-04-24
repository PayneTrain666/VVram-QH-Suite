#pragma once

#include <cstdint>

namespace holoram {

constexpr std::uint32_t HOLO_SPARSE_BLOCK_MAGIC = 0x48535042u; // HSPB
constexpr std::uint32_t HOLO_SPARSE_K_MAX = 64;

struct HoloSparseBlock {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloSparseBlock);
    std::uint32_t magic = HOLO_SPARSE_BLOCK_MAGIC;
    std::uint16_t q = 256;
    std::uint16_t k = 24;
    std::uint16_t qbits = 8;
    std::uint16_t flags = 0;
    float scale = 1.0f;
    std::uint16_t idx[HOLO_SPARSE_K_MAX]{};
    std::int16_t re[HOLO_SPARSE_K_MAX]{};
    std::int16_t im[HOLO_SPARSE_K_MAX]{};
};

} // namespace holoram
