#include "holoram/qh_codebook.h"

#include <cmath>
#include <sstream>

namespace holoram {

std::uint32_t qh_mix32(std::uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;
    return x;
}

std::uint32_t qh_gcd_u32(std::uint32_t a, std::uint32_t b) {
    while (b != 0) {
        const std::uint32_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

bool qh_is_power_of_two(std::uint32_t x) {
    return x != 0 && (x & (x - 1u)) == 0;
}

const char* to_string(HoloCodebookQuality quality) {
    switch (quality) {
        case HoloCodebookQuality::Unknown: return "Unknown";
        case HoloCodebookQuality::Good: return "Good";
        case HoloCodebookQuality::Warning: return "Warning";
        case HoloCodebookQuality::Failed: return "Failed";
        default: return "Invalid";
    }
}

HoloCoherenceReport HoloCodebook::measure_coherence() const {
    HoloCoherenceReport rep{};
    if (cfg_.channels < 2 || cfg_.q == 0 || codes_.empty()) return rep;

    double sum = 0.0;
    double sum_sq = 0.0;
    std::uint32_t pairs = 0;

    for (std::uint32_t i = 0; i < cfg_.channels; ++i) {
        const Complex32* ci = channel(i);
        for (std::uint32_t j = i + 1; j < cfg_.channels; ++j) {
            const Complex32* cj = channel(j);
            Complex32 acc(0.0f, 0.0f);
            for (std::uint32_t k = 0; k < cfg_.q; ++k) acc += std::conj(ci[k]) * cj[k];
            const float mu = std::abs(acc) / static_cast<float>(cfg_.q);
            sum += static_cast<double>(mu);
            sum_sq += static_cast<double>(mu) * static_cast<double>(mu);
            ++pairs;
            if (mu > rep.mu_max) {
                rep.mu_max = mu;
                rep.worst_i = i;
                rep.worst_j = j;
            }
        }
    }

    rep.pair_count = pairs;
    rep.mu_avg = pairs ? static_cast<float>(sum / static_cast<double>(pairs)) : 0.0f;
    rep.mu_rms = pairs ? static_cast<float>(std::sqrt(sum_sq / static_cast<double>(pairs))) : 0.0f;
    return rep;
}

HoloCodebookValidation HoloCodebook::validate() const {
    HoloCodebookValidation out{};
    out.dimensions_ok = cfg_.q > 0 && cfg_.channels > 0 && codes_.size() == static_cast<std::size_t>(cfg_.q) * cfg_.channels;
    if (!out.dimensions_ok) {
        out.quality = HoloCodebookQuality::Failed;
        out.message = "Invalid codebook dimensions";
        return out;
    }

    out.finite_ok = true;
    out.unit_magnitude_ok = true;
    constexpr float unit_tol = 2.5e-3f;

    for (const auto& z : codes_) {
        if (!std::isfinite(z.real()) || !std::isfinite(z.imag())) {
            out.finite_ok = false;
            break;
        }
        const float mag = std::abs(z);
        if (std::abs(mag - 1.0f) > unit_tol) out.unit_magnitude_ok = false;
    }

    if (!out.finite_ok) {
        out.quality = HoloCodebookQuality::Failed;
        out.message = "Codebook contains non-finite values";
        return out;
    }

    if (!out.unit_magnitude_ok) {
        out.quality = HoloCodebookQuality::Failed;
        out.message = "Codebook contains non-unit-magnitude values";
        return out;
    }

    out.coherence = measure_coherence();

    if (out.coherence.mu_max >= cfg_.fail_mu_max) {
        out.coherence_ok = false;
        out.quality = HoloCodebookQuality::Failed;
        std::ostringstream oss;
        oss << "Coherence failed: mu_max=" << out.coherence.mu_max
            << " fail_mu_max=" << cfg_.fail_mu_max
            << " worst_pair=(" << out.coherence.worst_i << "," << out.coherence.worst_j << ")";
        out.message = oss.str();
        return out;
    }

    if (out.coherence.mu_max >= cfg_.warn_mu_max) {
        out.coherence_ok = true;
        out.quality = HoloCodebookQuality::Warning;
        std::ostringstream oss;
        oss << "Coherence warning: mu_max=" << out.coherence.mu_max
            << " warn_mu_max=" << cfg_.warn_mu_max
            << " worst_pair=(" << out.coherence.worst_i << "," << out.coherence.worst_j << ")";
        out.message = oss.str();
        return out;
    }

    out.coherence_ok = true;
    out.quality = HoloCodebookQuality::Good;
    out.message = "Codebook validation passed";
    return out;
}

} // namespace holoram
