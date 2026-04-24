#include "holoram/qh_codebook.h"
#include <cmath>
#include <stdexcept>

namespace holoram {

static Complex32 cis(float x) { return Complex32(std::cos(x), std::sin(x)); }

HoloCodebook::HoloCodebook(HoloCodebookConfig cfg) : cfg_(cfg) {
    if (cfg_.q == 0 || cfg_.channels == 0) throw std::invalid_argument("HoloCodebook requires q > 0 and channels > 0");
    if (cfg_.strategy == HoloCodeStrategy::CazacZc) build_cazac_zc(); else build_mub_dft();
}

const Complex32* HoloCodebook::channel(std::uint32_t c) const { return &codes_.at(c * cfg_.q); }

void HoloCodebook::build_cazac_zc() {
    codes_.resize(static_cast<std::size_t>(cfg_.channels) * cfg_.q);
    for (std::uint32_t c = 0; c < cfg_.channels; ++c) {
        std::uint32_t root = 1 + 2 * c + cfg_.salt;
        if (root == 0) root = 1;
        for (std::uint32_t k = 0; k < cfg_.q; ++k) {
            float phase = -3.14159265358979323846f * static_cast<float>(root) * static_cast<float>(k) * static_cast<float>(k + 1) / static_cast<float>(cfg_.q);
            codes_[static_cast<std::size_t>(c) * cfg_.q + k] = cis(phase);
        }
    }
}

} // namespace holoram
