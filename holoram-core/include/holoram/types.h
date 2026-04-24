#pragma once

#include <complex>
#include <cstdint>

namespace holoram {

using HoloSlotId = std::uint32_t;
using HoloChannelId = std::uint16_t;
using HoloGeneration = std::uint32_t;
using HoloLogicalId = std::uint64_t;
using Complex32 = std::complex<float>;

enum class HoloCodeStrategy : std::uint16_t { CazacZc = 1, MubDft = 2 };
enum class HoloDemixMode : std::uint16_t { Matched = 1, Mmse = 2, MmseSic = 3 };
enum class HoloQuantMode : std::uint16_t { None = 0, Int8Complex = 1, Int16Complex = 2, Float16Complex = 3 };
enum class HoloCodecMode : std::uint16_t { ExactRaw = 1, ExactQhResidual = 2, ApproxVisual = 3, ApproxVector = 4, ApproxModel = 5 };
enum class HoloQualityMode : std::uint16_t { Exact = 1, High = 2, Balanced = 3, Aggressive = 4 };
enum class HoloResidualCompression : std::uint16_t { None = 0, Rle = 1, AutoRle = 2 };

inline bool is_exact_codec_mode(HoloCodecMode mode) {
    return mode == HoloCodecMode::ExactRaw || mode == HoloCodecMode::ExactQhResidual;
}

struct HoloCoreConfig {
    std::uint32_t q = 256;
    std::uint32_t channels = 16;
    std::uint32_t slots = 1u << 20;
    HoloCodeStrategy code_strategy = HoloCodeStrategy::CazacZc;
    HoloDemixMode demix = HoloDemixMode::Mmse;
    HoloQuantMode quant = HoloQuantMode::Int8Complex;
    std::uint32_t k_sparse = 24;
    float lambda = 1e-2f;
    std::uint32_t sic_iters = 2;
    float write_scale = 0.05f;
    float slot_energy_cap = 1.0f;
};

} // namespace holoram
