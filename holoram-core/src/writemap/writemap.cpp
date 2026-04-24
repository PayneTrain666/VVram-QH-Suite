#include "holoram/qh_writemap.h"

namespace holoram {

void HoloWriteMap::put(const HoloMapRecord& rec) { records_[rec.key] = rec; }

std::optional<HoloMapRecord> HoloWriteMap::get(const HoloMapKey& key) const {
    auto it = records_.find(key);
    if (it == records_.end()) return std::nullopt;
    return it->second;
}

bool HoloWriteMap::erase(const HoloMapKey& key) { return records_.erase(key) > 0; }

} // namespace holoram
