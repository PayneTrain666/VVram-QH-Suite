#pragma once

#include "holovram/hv_tile.h"
#include <cstdint>
#include <vector>

namespace holovram {

struct TileRequest { ResourceId resource_id = 0; TileCoord coord{}; std::uint64_t target_frame = 0; float priority = 1.0f; };

class PrefetchScheduler {
public:
    void submit(TileRequest req);
    std::vector<TileRequest> drain_ready(std::uint64_t frame_id);
private:
    std::vector<TileRequest> queue_;
};

} // namespace holovram
