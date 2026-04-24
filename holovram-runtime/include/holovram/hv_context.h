#pragma once

#include "holovram/hv_resource.h"
#include "holovram/hv_tile.h"
#include "holovram/hv_prefetch.h"
#include "holovram/hv_metrics.h"
#include <memory>

namespace holovram {

struct ContextConfig { std::uint64_t simulated_vram_bytes = 512ull * 1024ull * 1024ull; std::uint64_t qh_warm_bytes = 4ull * 1024ull * 1024ull * 1024ull; };

class Context {
public:
    explicit Context(ContextConfig cfg);
    ResourceHandle create_resource(const ResourceDesc& desc);
    bool upload_tile(ResourceHandle resource, TileCoord coord, const void* data, std::uint32_t bytes);
    bool prefetch_tiles(const TileRequest* requests, std::uint32_t count, std::uint64_t target_frame);
    FrameMetrics end_frame(std::uint64_t frame_id);
private:
    ContextConfig cfg_{};
    ResourceId next_resource_id_ = 1;
};

std::unique_ptr<Context> create_context(ContextConfig cfg);

} // namespace holovram
