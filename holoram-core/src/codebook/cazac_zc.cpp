#include "holoram/qh_codebook.h"

#include <cmath>
#include <stdexcept>

namespace holoram {

namespace {

constexpr float kPi = 3.14159265358979323846f;

Complex32 cis(float phase) {
    return Complex32(std::cos(phase), std::sin(phase));
}

std::uint32_t make_odd(std::uint32_t x) {
    return x | 1u;
}

std::uint32_t choose_coprime_root(std::uint32_t q, std::uint32_t channel, std::uint32_t seed, std::uint32_t salt)
{
    std::uint32_t x = seed;
    x ^= salt + 0x9e3779b9u;
    x ^= channel * 0x85ebca6bu;
    x = qh_mix32(x);

    std::uint32_t root = (x % q);
    if (root == 0) root = 1;
    root = make_odd(root);

    for (std::uint32_t attempt = 0; attempt < q; ++attempt) {
        const std::uint32_t candidate = (root + 2u * attempt) % q;
        const std::uint32_t safe_candidate = candidate == 0 ? 1u : candidate;
        if (qh_gcd_u32(safe_candidate, q) == 1u) return safe_candidate;
    }

    return 1u;
}

} // namespace

HoloCodebook::HoloCodebook(HoloCodebookConfig cfg) : cfg_(cfg) {
    if (cfg_.q == 0 || cfg_.channels == 0) throw std::invalid_argument("HoloCodebook requires q > 0 and channels > 0");
    if (cfg_.q < 8) throw std::invalid_argument("HoloCodebook q is too small; minimum supported q is 8");

    if (cfg_.strategy == HoloCodeStrategy::CazacZc) build_cazac_zc();
    else if (cfg_.strategy == HoloCodeStrategy::MubDft) build_mub_dft();
    else throw std::invalid_argument("Unsupported HoloCodeStrategy");

    if (cfg_.strict_validation) {
        const auto validation = validate();
        if (validation.quality == HoloCodebookQuality::Failed) {
            throw std::runtime_error("HoloCodebook validation failed: " + validation.message);
        }
    }
}

const Complex32* HoloCodebook::channel(std::uint32_t c) const {
    if (c >= cfg_.channels) throw std::out_of_range("HoloCodebook channel index out of range");
    return &codes_.at(static_cast<std::size_t>(c) * cfg_.q);
}

void HoloCodebook::build_cazac_zc() {
    codes_.assign(static_cast<std::size_t>(cfg_.channels) * cfg_.q, Complex32{1.0f, 0.0f});

    for (std::uint32_t c = 0; c < cfg_.channels; ++c) {
        const std::uint32_t root = choose_coprime_root(cfg_.q, c, cfg_.seed, cfg_.salt);
        const std::uint32_t phase_seed = qh_mix32(cfg_.seed ^ (cfg_.salt * 0x9e3779b9u) ^ (c * 0x7feb352du));
        const float phase_offset = 2.0f * kPi * static_cast<float>(phase_seed & 0xFFFFu) / static_cast<float>(0x10000u);

        for (std::uint32_t k = 0; k < cfg_.q; ++k) {
            const float phase = -kPi * static_cast<float>(root) * static_cast<float>(k) * static_cast<float>(k + 1u) / static_cast<float>(cfg_.q) + phase_offset;
            codes_[static_cast<std::size_t>(c) * cfg_.q + k] = cis(phase);
        }
    }
}

} // namespace holoram
