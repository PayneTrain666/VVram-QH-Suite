#pragma once

#include "holoram/errors.h"
#include "holoram/types.h"

#include <cstdint>
#include <string>
#include <vector>

namespace holoram {

enum class HoloCodebookQuality : std::uint16_t {
    Unknown = 0,
    Good = 1,
    Warning = 2,
    Failed = 3
};

struct HoloCodebookConfig {
    std::uint32_t q = 256;
    std::uint32_t channels = 16;

    HoloCodeStrategy strategy = HoloCodeStrategy::CazacZc;

    // Deterministic generation controls.
    // Same config => same codebook. Different seed or salt => different codebook.
    std::uint32_t seed = 1234;
    std::uint32_t salt = 0;

    // Optional quality gates. Conservative defaults for early testing.
    float warn_mu_max = 0.20f;
    float fail_mu_max = 0.45f;

    // When true, constructor throws if generated codebook fails validation.
    bool strict_validation = true;
};

struct HoloCoherenceReport {
    float mu_max = 0.0f;
    float mu_avg = 0.0f;
    float mu_rms = 0.0f;
    std::uint32_t worst_i = 0;
    std::uint32_t worst_j = 0;
    std::uint32_t pair_count = 0;
};

struct HoloCodebookValidation {
    HoloCodebookQuality quality = HoloCodebookQuality::Unknown;
    bool dimensions_ok = false;
    bool unit_magnitude_ok = false;
    bool finite_ok = false;
    bool coherence_ok = false;
    HoloCoherenceReport coherence{};
    std::string message;
};

class HoloCodebook {
public:
    HoloCodebook() = default;
    explicit HoloCodebook(HoloCodebookConfig cfg);

    const HoloCodebookConfig& config() const { return cfg_; }
    const std::vector<Complex32>& codes() const { return codes_; }
    const Complex32* channel(std::uint32_t c) const;
    HoloCoherenceReport measure_coherence() const;
    HoloCodebookValidation validate() const;
    bool empty() const { return codes_.empty(); }

private:
    HoloCodebookConfig cfg_{};
    std::vector<Complex32> codes_;
    void build_cazac_zc();
    void build_mub_dft();
};

std::uint32_t qh_mix32(std::uint32_t x);
std::uint32_t qh_gcd_u32(std::uint32_t a, std::uint32_t b);
bool qh_is_power_of_two(std::uint32_t x);
const char* to_string(HoloCodebookQuality quality);

} // namespace holoram
