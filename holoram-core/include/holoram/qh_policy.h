#pragma once

#include "holoram/types.h"
#include <cstdint>

namespace holoram {

struct HoloPolicyConfig {
    std::uint16_t channels = 16;
    std::uint16_t depth_slices = 8;
    std::uint32_t epoch = 1;
    std::uint32_t max_redirect_tries = 16;
    std::uint32_t slot_budget_init = 1024;
    std::uint32_t slot_budget_hot = 128;
    std::uint32_t slot_budget_deny = 32;
};

std::uint32_t mix32(std::uint32_t x);
HoloChannelId channel_for(HoloSlotId slot_id, std::uint16_t block_idx, std::uint32_t epoch, std::uint16_t depth, std::uint16_t channels);

} // namespace holoram
