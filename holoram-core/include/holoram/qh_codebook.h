#pragma once

#include "holoram/types.h"
#include <cstdint>
#include <vector>

namespace holoram {

struct HoloCodebookConfig {
    std::uint32_t q = 256;
    std::uint32_t channels = 16;
    HoloCodeStrategy strategy = HoloCodeStrategy::CazacZc;
    std::uint32_t seed = 1234;
    std::uint32_t salt = 0;
};

struct HoloCoherenceReport {
    float mu_max = 0.0f;
    float mu_avg = 0.0f;
    std::uint32_t worst_i = 0;
    std::uint32_t worst_j = 0;
};

class HoloCodebook {
public:
    HoloCodebook() = default;
    explicit HoloCodebook(HoloCodebookConfig cfg);
    const HoloCodebookConfig& config() const { return cfg_; }
    const std::vector<Complex32>& codes() const { return codes_; }
    const Complex32* channel(std::uint32_t c) const;
    HoloCoherenceReport measure_coherence() const;
private:
    HoloCodebookConfig cfg_{};
    std::vector<Complex32> codes_;
    void build_cazac_zc();
    void build_mub_dft();
};

} // namespace holoram
