#pragma once

#include <cstdint>

namespace holoram {

struct HoloTileMapRecord {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloTileMapRecord);
    std::uint64_t resource_id = 0;
    std::uint32_t tile_id = 0;
    std::uint32_t mip = 0;
    std::uint32_t tile_x = 0;
    std::uint32_t tile_y = 0;
    std::uint32_t tile_z = 0;
    std::uint32_t slot_id = 0;
    std::uint16_t channel_base = 0;
    std::uint16_t k_sparse = 0;
    std::uint32_t generation = 0;
    std::uint32_t flags = 0;
};

} // namespace holoram
