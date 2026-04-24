#pragma once

#include <cstdint>

namespace holovram {

enum class ResidencyState : std::uint16_t { ResidentVRAM = 1, ResidentQHSystem = 2, ResidentRawSystem = 3, ResidentDisk = 4, DecodePending = 5, UploadPending = 6, Evicted = 7, Failed = 8 };

struct ResidencyStats {
    std::uint64_t tile_faults = 0;
    std::uint64_t vram_hits = 0;
    std::uint64_t qh_hits = 0;
    std::uint64_t disk_hits = 0;
    std::uint64_t evictions = 0;
};

} // namespace holovram
