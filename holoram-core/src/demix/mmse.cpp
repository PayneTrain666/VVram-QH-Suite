#include "holoram/qh_demix.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace holoram {

namespace {

bool solve_complex_linear_system(
    std::vector<Complex32> a,
    std::vector<Complex32> b,
    std::uint32_t n,
    std::vector<Complex32>& x)
{
    x.assign(n, Complex32{0.0f, 0.0f});

    for (std::uint32_t col = 0; col < n; ++col) {
        std::uint32_t pivot = col;
        float best = std::abs(a[col * n + col]);

        for (std::uint32_t row = col + 1; row < n; ++row) {
            const float cand = std::abs(a[row * n + col]);
            if (cand > best) {
                best = cand;
                pivot = row;
            }
        }

        if (best < 1e-12f) {
            return false;
        }

        if (pivot != col) {
            for (std::uint32_t j = 0; j < n; ++j) {
                std::swap(a[col * n + j], a[pivot * n + j]);
            }
            std::swap(b[col], b[pivot]);
        }

        const Complex32 diag = a[col * n + col];

        for (std::uint32_t j = col; j < n; ++j) {
            a[col * n + j] /= diag;
        }
        b[col] /= diag;

        for (std::uint32_t row = 0; row < n; ++row) {
            if (row == col) {
                continue;
            }

            const Complex32 factor = a[row * n + col];
            if (std::abs(factor) == 0.0f) {
                continue;
            }

            for (std::uint32_t j = col; j < n; ++j) {
                a[row * n + j] -= factor * a[col * n + j];
            }
            b[row] -= factor * b[col];
        }
    }

    x = std::move(b);
    return true;
}

} // namespace

HoloDemixResult mmse_demix(
    const std::vector<Complex32>& observation,
    const HoloCodebook& codebook,
    float lambda)
{
    HoloDemixResult out{};
    out.mode = HoloDemixMode::Mmse;

    const auto q = codebook.config().q;
    const auto channels = codebook.config().channels;

    if (observation.size() != q || q == 0 || channels == 0) {
        out.error = HoloError::InvalidArgument;
        return out;
    }

    lambda = std::max(lambda, 0.0f);

    std::vector<Complex32> gram(static_cast<std::size_t>(channels) * channels, Complex32{0.0f, 0.0f});
    std::vector<Complex32> rhs(channels, Complex32{0.0f, 0.0f});

    for (std::uint32_t i = 0; i < channels; ++i) {
        const auto* ci = codebook.channel(i);

        for (std::uint32_t j = 0; j < channels; ++j) {
            const auto* cj = codebook.channel(j);
            Complex32 acc{0.0f, 0.0f};

            for (std::uint32_t k = 0; k < q; ++k) {
                acc += std::conj(ci[k]) * cj[k];
            }

            gram[static_cast<std::size_t>(i) * channels + j] = acc / static_cast<float>(q);
        }

        Complex32 b{0.0f, 0.0f};
        for (std::uint32_t k = 0; k < q; ++k) {
            b += std::conj(ci[k]) * observation[k];
        }
        rhs[i] = b / static_cast<float>(q);
    }

    for (std::uint32_t i = 0; i < channels; ++i) {
        gram[static_cast<std::size_t>(i) * channels + i] += Complex32{lambda, 0.0f};
    }

    if (!solve_complex_linear_system(gram, rhs, channels, out.channel_values)) {
        out.error = HoloError::DecodeFailed;
        return out;
    }

    out.reconstructed = synthesize_from_channel_values(codebook, out.channel_values);
    out.residual_norm = residual_l2_norm(observation, out.reconstructed);
    out.iterations = 1;
    out.error = HoloError::Ok;
    return out;
}

} // namespace holoram
