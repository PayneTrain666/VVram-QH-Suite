#pragma once

#include "holovram/hv_tile.h"

namespace holovram {

class GraphicsBackend {
public:
    virtual ~GraphicsBackend() = default;
    virtual bool make_tile_resident(const TileDesc& tile, const void* decoded_data) = 0;
    virtual bool evict_tile(const TileDesc& tile) = 0;
};

} // namespace holovram
