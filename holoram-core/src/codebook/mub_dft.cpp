#include "holoram/qh_codebook.h"

#include <cmath>

namespace holoram {

namespace {

constexpr float kPi = 3.14159265358979323846f;

Complex32 cis_mub(float phase) {
    return Complex32(std::cos(phase), std::sin(phase));
}

} // namespace

void HoloCodebook::build_mub_dft() {
    codes_.assign(static_cast<std::size_t>(cfg_.channels) * cfg_.q, Complex32{1.0f, 0.0f});

    for (std::uint32_t c = 0; c < cfg_.channels; ++c) {
        const std::uint32_t h0 = qh_mix32(cfg_.seed ^ (cfg_.salt + 0x9e3779b9u) ^ (c * 0x85ebca6bu));
        const std::uint32_t h1 = qh_mix32(h0 ^ 0xc2b2ae35u);
        const std::uint32_t slope = 1u + (h0 % (cfg_.q - 1u));
        const std::uint32_t chirp = 1u + (h1 % (cfg_.q - 1u));
        const float phase_offset = 2.0f * kPi * static_cast<float>((h1 >> 8u) & 0xFFFFu) / static_cast<float>(0x10000u);

        for (std::uint32_t k = 0; k < cfg_.q; ++k) {
            const float linear_phase = 2.0f * kPi * static_cast<float>((static_cast<std::uint64_t>(slope) * k) % cfg_.q) / static_cast<float>(cfg_.q);
            const float quadratic_phase = kPi * static_cast<float>((static_cast<std::uint64_t>(chirp) * k * k) % cfg_.q) / static_cast<float>(cfg_.q);
            codes_[static_cast<std::size_t>(c) * cfg_.q + k] = cis_mub(linear_phase + quadratic_phase + phase_offset);
        }
    }
}

} // namespace holoram
