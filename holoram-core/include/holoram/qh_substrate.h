#pragma once

#include "holoram/types.h"
#include "holoram/errors.h"
#include <cstdint>
#include <memory>

namespace holoram {

struct HoloSlotStats {
    HoloSlotId slot_id = 0;
    float energy = 0.0f;
    float energy_cap = 0.0f;
    std::uint32_t writes = 0;
    std::uint32_t reads = 0;
    std::uint32_t generation = 0;
};

class HoloSubstrate {
public:
    virtual ~HoloSubstrate() = default;
    virtual HoloError write_block(HoloSlotId slot_id, HoloChannelId channel_id, const void* decoded_block, std::uint32_t decoded_bytes, HoloQualityMode quality) = 0;
    virtual HoloError read_block(HoloSlotId slot_id, HoloChannelId channel_id, void* out_decoded, std::uint32_t out_bytes, HoloQualityMode quality) = 0;
    virtual HoloError erase_slot(HoloSlotId slot_id) = 0;
    virtual HoloSlotStats get_slot_stats(HoloSlotId slot_id) const = 0;
};

std::unique_ptr<HoloSubstrate> create_cpu_substrate(const HoloCoreConfig& cfg);

} // namespace holoram
