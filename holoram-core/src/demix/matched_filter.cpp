#include "holoram/qh_demix.h"

#include <cmath>

namespace holoram {

std::vector<Complex32> synthesize_from_channel_values(
    const HoloCodebook& codebook,
    const std::vector<Complex32>& channel_values)
{
    const auto q = codebook.config().q;
    const auto channels = codebook.config().channels;
    std::vector<Complex32> out(q, Complex32{0.0f, 0.0f});

    const auto usable = std::min<std::size_t>(channel_values.size(), channels);

    for (std::size_t c = 0; c < usable; ++c) {
        const auto* code = codebook.channel(static_cast<std::uint32_t>(c));
        for (std::uint32_t k = 0; k < q; ++k) {
            out[k] += code[k] * channel_values[c];
        }
    }

    return out;
}

float residual_l2_norm(
    const std::vector<Complex32>& a,
    const std::vector<Complex32>& b)
{
    if (a.size() != b.size()) {
        return std::numeric_limits<float>::infinity();
    }

    double acc = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const auto d = a[i] - b[i];
        acc += static_cast<double>(std::norm(d));
    }

    return static_cast<float>(std::sqrt(acc));
}

HoloDemixResult matched_filter_demix(
    const std::vector<Complex32>& observation,
    const HoloCodebook& codebook)
{
    HoloDemixResult out{};
    out.mode = HoloDemixMode::Matched;

    const auto q = codebook.config().q;
    const auto channels = codebook.config().channels;

    if (observation.size() != q || q == 0 || channels == 0) {
        out.error = HoloError::InvalidArgument;
        return out;
    }

    out.channel_values.assign(channels, Complex32{0.0f, 0.0f});

    for (std::uint32_t c = 0; c < channels; ++c) {
        const auto* code = codebook.channel(c);
        Complex32 acc{0.0f, 0.0f};

        for (std::uint32_t k = 0; k < q; ++k) {
            acc += std::conj(code[k]) * observation[k];
        }

        out.channel_values[c] = acc / static_cast<float>(q);
    }

    out.reconstructed = synthesize_from_channel_values(codebook, out.channel_values);
    out.residual_norm = residual_l2_norm(observation, out.reconstructed);
    out.iterations = 1;
    out.error = HoloError::Ok;
    return out;
}

} // namespace holoram
