#include "holovram/hv_prefetch.h"

namespace holovram {

void PrefetchScheduler::submit(TileRequest req) { queue_.push_back(req); }

std::vector<TileRequest> PrefetchScheduler::drain_ready(std::uint64_t frame_id) {
    std::vector<TileRequest> ready;
    std::vector<TileRequest> keep;
    for (const auto& req : queue_) {
        if (req.target_frame <= frame_id) ready.push_back(req);
        else keep.push_back(req);
    }
    queue_ = std::move(keep);
    return ready;
}

} // namespace holovram
