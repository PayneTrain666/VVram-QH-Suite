#include "holoram/qh_policy.h"

namespace holoram {

std::uint32_t mix32(std::uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

HoloChannelId channel_for(HoloSlotId slot_id, std::uint16_t block_idx, std::uint32_t epoch, std::uint16_t depth, std::uint16_t channels) {
    if (channels == 0) return 0;
    std::uint32_t x = slot_id ^ (epoch + 0x9e3779b9U);
    x ^= static_cast<std::uint32_t>(depth) * 0x85ebca6bU;
    x ^= static_cast<std::uint32_t>(block_idx) * 0xc2b2ae35U;
    x = mix32(x);
    return static_cast<HoloChannelId>(x % channels);
}

} // namespace holoram
