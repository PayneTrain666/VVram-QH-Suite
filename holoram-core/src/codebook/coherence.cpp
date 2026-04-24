#include "holoram/qh_codebook.h"
#include <cmath>

namespace holoram {

HoloCoherenceReport HoloCodebook::measure_coherence() const {
    HoloCoherenceReport rep{};
    if (cfg_.channels < 2 || cfg_.q == 0) return rep;
    float sum = 0.0f;
    std::uint32_t pairs = 0;
    for (std::uint32_t i = 0; i < cfg_.channels; ++i) {
        const Complex32* ci = channel(i);
        for (std::uint32_t j = i + 1; j < cfg_.channels; ++j) {
            const Complex32* cj = channel(j);
            Complex32 acc(0.0f, 0.0f);
            for (std::uint32_t k = 0; k < cfg_.q; ++k) acc += std::conj(ci[k]) * cj[k];
            float mu = std::abs(acc) / static_cast<float>(cfg_.q);
            sum += mu;
            ++pairs;
            if (mu > rep.mu_max) { rep.mu_max = mu; rep.worst_i = i; rep.worst_j = j; }
        }
    }
    rep.mu_avg = pairs ? sum / static_cast<float>(pairs) : 0.0f;
    return rep;
}

} // namespace holoram
