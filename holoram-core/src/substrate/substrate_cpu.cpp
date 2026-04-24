#include "holoram/qh_substrate.h"
#include <cstring>
#include <memory>
#include <unordered_map>
#include <vector>

namespace holoram {

class CpuSubstrate final : public HoloSubstrate {
public:
    explicit CpuSubstrate(HoloCoreConfig cfg) : cfg_(cfg) {}

    HoloError write_block(HoloSlotId slot_id, HoloChannelId, const void* decoded_block, std::uint32_t decoded_bytes, HoloQualityMode) override {
        auto& slot = slots_[slot_id];
        slot.bytes.resize(decoded_bytes);
        std::memcpy(slot.bytes.data(), decoded_block, decoded_bytes);
        slot.stats.slot_id = slot_id;
        slot.stats.writes++;
        slot.stats.energy = static_cast<float>(decoded_bytes);
        slot.stats.energy_cap = cfg_.slot_energy_cap;
        slot.stats.generation++;
        return HoloError::Ok;
    }

    HoloError read_block(HoloSlotId slot_id, HoloChannelId, void* out_decoded, std::uint32_t out_bytes, HoloQualityMode) override {
        auto it = slots_.find(slot_id);
        if (it == slots_.end()) return HoloError::SlotNotFound;
        const auto& bytes = it->second.bytes;
        if (out_bytes < bytes.size()) return HoloError::InvalidArgument;
        std::memcpy(out_decoded, bytes.data(), bytes.size());
        it->second.stats.reads++;
        return HoloError::Ok;
    }

    HoloError erase_slot(HoloSlotId slot_id) override { slots_.erase(slot_id); return HoloError::Ok; }

    HoloSlotStats get_slot_stats(HoloSlotId slot_id) const override {
        auto it = slots_.find(slot_id);
        if (it == slots_.end()) { HoloSlotStats stats{}; stats.slot_id = slot_id; return stats; }
        return it->second.stats;
    }

private:
    struct Slot { std::vector<std::uint8_t> bytes; HoloSlotStats stats; };
    HoloCoreConfig cfg_;
    mutable std::unordered_map<HoloSlotId, Slot> slots_;
};

std::unique_ptr<HoloSubstrate> create_cpu_substrate(const HoloCoreConfig& cfg) { return std::make_unique<CpuSubstrate>(cfg); }

} // namespace holoram
