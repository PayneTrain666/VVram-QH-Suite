#pragma once

#include "holovram/hv_resource.h"
#include "holovram/hv_residency.h"
#include <cstdint>

namespace holovram {

struct TileCoord { std::uint32_t mip = 0; std::uint32_t x = 0; std::uint32_t y = 0; std::uint32_t z = 0; };

struct TileDesc {
    ResourceId resource_id = 0;
    TileId tile_id = 0;
    TileCoord coord{};
    std::uint32_t decoded_bytes = 0;
    std::uint32_t vram_bytes = 0;
    std::uint32_t qh_slot_id = 0;
    std::uint16_t channel_base = 0;
    std::uint16_t k_sparse = 24;
    std::uint32_t generation = 0;
    ResidencyState state = ResidencyState::Evicted;
};

} // namespace holovram
