#include "holoram/qh_codebook.h"
#include <cmath>

namespace holoram {

static Complex32 cis_mub(float x) { return Complex32(std::cos(x), std::sin(x)); }

void HoloCodebook::build_mub_dft() {
    codes_.resize(static_cast<std::size_t>(cfg_.channels) * cfg_.q);
    for (std::uint32_t c = 0; c < cfg_.channels; ++c) {
        float phi = 2.0f * 3.14159265358979323846f * static_cast<float>(c + cfg_.salt) / static_cast<float>(cfg_.channels);
        for (std::uint32_t k = 0; k < cfg_.q; ++k) {
            float phase = 2.0f * 3.14159265358979323846f * static_cast<float>(k) * static_cast<float>(c + 1) / static_cast<float>(cfg_.q) + phi;
            codes_[static_cast<std::size_t>(c) * cfg_.q + k] = cis_mub(phase);
        }
    }
}

} // namespace holoram
