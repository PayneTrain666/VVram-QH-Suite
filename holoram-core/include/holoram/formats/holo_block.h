#pragma once

#include <cstdint>

namespace holoram {

constexpr std::uint32_t HOLO_BLOCK_MAGIC = 0x48424C4Bu; // HBLK

struct HoloBlockHeader {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloBlockHeader);
    std::uint32_t magic = HOLO_BLOCK_MAGIC;
    std::uint32_t flags = 0;
    std::uint32_t q = 256;
    std::uint32_t channels = 16;
    std::uint16_t codec_mode = 0;
    std::uint16_t quant_mode = 0;
    std::uint16_t demix_mode = 0;
    std::uint16_t reserved0 = 0;
    std::uint32_t decoded_bytes = 0;
    std::uint32_t payload_bytes = 0;
    std::uint32_t generation = 0;
    std::uint32_t crc32 = 0;
};

} // namespace holoram
