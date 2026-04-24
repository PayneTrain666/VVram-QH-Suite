#include "holoram/qh_codebook.h"
#include <cassert>
#include <cmath>

int main() {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = 256;
    cfg.channels = 16;
    holoram::HoloCodebook cb(cfg);
    for (std::uint32_t c = 0; c < cfg.channels; ++c) {
        const auto* ch = cb.channel(c);
        for (std::uint32_t k = 0; k < cfg.q; ++k) {
            float mag = std::abs(ch[k]);
            assert(std::abs(mag - 1.0f) < 1e-3f);
        }
    }
    auto rep = cb.measure_coherence();
    assert(rep.mu_max >= 0.0f);
    return 0;
}
