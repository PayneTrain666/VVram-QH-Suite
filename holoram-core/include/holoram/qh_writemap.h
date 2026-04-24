#pragma once

#include "holoram/types.h"
#include "holoram/formats/holo_handle.h"
#include <cstdint>
#include <optional>
#include <unordered_map>

namespace holoram {

struct HoloMapKey {
    HoloLogicalId logical_id = 0;
    std::uint32_t sub_id = 0;
    bool operator==(const HoloMapKey& other) const { return logical_id == other.logical_id && sub_id == other.sub_id; }
};

struct HoloMapKeyHash {
    std::size_t operator()(const HoloMapKey& k) const {
        return static_cast<std::size_t>(k.logical_id ^ (static_cast<std::uint64_t>(k.sub_id) << 32));
    }
};

struct HoloMapRecord {
    HoloMapKey key{};
    HoloHandle handle{};
};

class HoloWriteMap {
public:
    void put(const HoloMapRecord& rec);
    std::optional<HoloMapRecord> get(const HoloMapKey& key) const;
    bool erase(const HoloMapKey& key);
private:
    std::unordered_map<HoloMapKey, HoloMapRecord, HoloMapKeyHash> records_;
};

} // namespace holoram
