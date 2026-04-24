#include "holovram/hv_context.h"

namespace holovram {

Context::Context(ContextConfig cfg) : cfg_(cfg) {}

ResourceHandle Context::create_resource(const ResourceDesc&) { return ResourceHandle{next_resource_id_++}; }

bool Context::upload_tile(ResourceHandle, TileCoord, const void*, std::uint32_t) {
    // TODO(v0.1): store tile into QH backing.
    return true;
}

bool Context::prefetch_tiles(const TileRequest*, std::uint32_t, std::uint64_t) {
    // TODO(v0.1): queue tile prefetch requests.
    return true;
}

FrameMetrics Context::end_frame(std::uint64_t) { return FrameMetrics{}; }

std::unique_ptr<Context> create_context(ContextConfig cfg) { return std::make_unique<Context>(cfg); }

} // namespace holovram
