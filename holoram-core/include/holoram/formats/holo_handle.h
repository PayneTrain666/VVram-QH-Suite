#pragma once

#include <cstdint>

namespace holoram {

struct HoloHandle {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloHandle);
    std::uint32_t slot_id = 0;
    std::uint16_t channel_base = 0;
    std::uint16_t profile_id = 0;
    std::uint16_t k_sparse = 0;
    std::uint16_t codec_mode = 0;
    std::uint32_t generation = 0;
    std::uint32_t crc32 = 0;
    std::uint32_t decoded_bytes = 0;
    std::uint32_t flags = 0;
};

} // namespace holoram
