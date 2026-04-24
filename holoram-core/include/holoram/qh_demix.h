#pragma once

#include "holoram/errors.h"
#include "holoram/qh_codebook.h"
#include "holoram/types.h"

#include <cstdint>
#include <vector>

namespace holoram {

struct HoloDemixConfig {
    HoloDemixMode mode = HoloDemixMode::Matched;
    float lambda = 1e-2f;
    std::uint32_t sic_iters = 2;
    float stop_residual = 1e-6f;
};

struct HoloDemixResult {
    HoloError error = HoloError::Ok;
    HoloDemixMode mode = HoloDemixMode::Matched;
    std::vector<Complex32> channel_values;
    double residual_norm = 0.0;
    std::uint32_t iterations = 0;
};

HoloDemixResult matched_filter_demix(
    const std::vector<Complex32>& y,
    const HoloCodebook& codebook,
    const HoloDemixConfig& cfg);

HoloDemixResult mmse_demix(
    const std::vector<Complex32>& y,
    const HoloCodebook& codebook,
    const HoloDemixConfig& cfg);

HoloDemixResult sic_demix(
    const std::vector<Complex32>& y,
    const HoloCodebook& codebook,
    const HoloDemixConfig& cfg);

HoloDemixResult demix_channels(
    const std::vector<Complex32>& y,
    const HoloCodebook& codebook,
    const HoloDemixConfig& cfg);

std::vector<Complex32> synthesize_from_channel_values(
    const HoloCodebook& codebook,
    const std::vector<Complex32>& channel_values);

double residual_l2_norm(
    const std::vector<Complex32>& a,
    const std::vector<Complex32>& b);

} // namespace holoram
