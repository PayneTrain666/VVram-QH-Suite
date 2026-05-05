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
    bool use_gram_cache = true;
};

struct HoloDemixResult {
    HoloError error = HoloError::Ok;
    HoloDemixMode mode = HoloDemixMode::Matched;
    std::vector<Complex32> channel_values;
    std::vector<Complex32> reconstructed;
    float residual_norm = 0.0f;
    std::uint32_t iterations = 0;
    bool used_cache = false;
};

HoloDemixResult matched_filter_demix(const std::vector<Complex32>& observation, const HoloCodebook& codebook);
HoloDemixResult mmse_demix(const std::vector<Complex32>& observation, const HoloCodebook& codebook, float lambda);
HoloDemixResult mmse_demix_cached(const std::vector<Complex32>& observation, const HoloCodebook& codebook, float lambda, bool use_cache);
HoloDemixResult sic_demix(const std::vector<Complex32>& observation, const HoloCodebook& codebook, float lambda, std::uint32_t sic_iters);
HoloDemixResult demix_channels(const std::vector<Complex32>& observation, const HoloCodebook& codebook, const HoloDemixConfig& cfg);
std::vector<Complex32> synthesize_from_channel_values(const HoloCodebook& codebook, const std::vector<Complex32>& channel_values);
float residual_l2_norm(const std::vector<Complex32>& a, const std::vector<Complex32>& b);

} // namespace holoram
