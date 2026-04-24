#include "holoram/qh_demix.h"

#include <cassert>
#include <cmath>
#include <vector>

namespace {

float abs_err(holoram::Complex32 a, holoram::Complex32 b) {
    return std::abs(a - b);
}

} // namespace

int main() {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = 256;
    cfg.channels = 8;
    cfg.strategy = holoram::HoloCodeStrategy::MubDft;
    cfg.fail_mu_max = 0.75f;

    holoram::HoloCodebook cb(cfg);

    std::vector<holoram::Complex32> truth(cfg.channels);
    for (std::uint32_t c = 0; c < cfg.channels; ++c) {
        truth[c] = holoram::Complex32{0.1f * static_cast<float>(c + 1), -0.025f * static_cast<float>(c)};
    }

    const auto y = holoram::synthesize_from_channel_values(cb, truth);

    auto matched = holoram::matched_filter_demix(y, cb);
    assert(matched.error == holoram::HoloError::Ok);

    auto mmse = holoram::mmse_demix(y, cb, 1e-6f);
    assert(mmse.error == holoram::HoloError::Ok);
    assert(mmse.channel_values.size() == truth.size());

    for (std::uint32_t c = 0; c < cfg.channels; ++c) {
        assert(abs_err(mmse.channel_values[c], truth[c]) < 1e-2f);
    }

    assert(mmse.residual_norm <= matched.residual_norm + 1e-3f);
    return 0;
}
