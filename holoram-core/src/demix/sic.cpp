#include "holoram/qh_demix.h"

#include <algorithm>
#include <cmath>

namespace holoram {

HoloDemixResult sic_demix(
    const std::vector<Complex32>& observation,
    const HoloCodebook& codebook,
    float lambda,
    std::uint32_t sic_iters)
{
    HoloDemixResult out{};
    out.mode = HoloDemixMode::MmseSic;

    auto base = mmse_demix(observation, codebook, lambda);
    if (base.error != HoloError::Ok) {
        return base;
    }

    out.channel_values = base.channel_values;

    const auto channels = codebook.config().channels;
    const auto q = codebook.config().q;

    std::vector<Complex32> residual = observation;
    auto reconstructed = synthesize_from_channel_values(codebook, out.channel_values);

    for (std::uint32_t k = 0; k < q; ++k) {
        residual[k] -= reconstructed[k];
    }

    const auto passes = std::max<std::uint32_t>(1, sic_iters);

    for (std::uint32_t iter = 0; iter < passes; ++iter) {
        for (std::uint32_t c = 0; c < channels; ++c) {
            const auto* code = codebook.channel(c);
            Complex32 corr{0.0f, 0.0f};

            for (std::uint32_t k = 0; k < q; ++k) {
                corr += std::conj(code[k]) * residual[k];
            }

            corr /= static_cast<float>(q);
            corr /= (1.0f + std::max(lambda, 0.0f));

            out.channel_values[c] += corr;

            for (std::uint32_t k = 0; k < q; ++k) {
                residual[k] -= code[k] * corr;
            }
        }
    }

    out.reconstructed = synthesize_from_channel_values(codebook, out.channel_values);
    out.residual_norm = residual_l2_norm(observation, out.reconstructed);
    out.iterations = passes;
    out.error = HoloError::Ok;
    return out;
}

HoloDemixResult demix_channels(
    const std::vector<Complex32>& observation,
    const HoloCodebook& codebook,
    const HoloDemixConfig& cfg)
{
    switch (cfg.mode) {
        case HoloDemixMode::Matched:
            return matched_filter_demix(observation, codebook);
        case HoloDemixMode::Mmse:
            return mmse_demix(observation, codebook, cfg.lambda);
        case HoloDemixMode::MmseSic:
            return sic_demix(observation, codebook, cfg.lambda, cfg.sic_iters);
        default: {
            HoloDemixResult out{};
            out.error = HoloError::UnsupportedMode;
            return out;
        }
    }
}

} // namespace holoram
