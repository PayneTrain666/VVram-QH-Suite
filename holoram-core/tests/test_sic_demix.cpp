#include "holoram/qh_demix.h"

#include <cassert>
#include <vector>

int main() {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = 512;
    cfg.channels = 16;
    cfg.strategy = holoram::HoloCodeStrategy::MubDft;
    cfg.fail_mu_max = 0.75f;

    holoram::HoloCodebook cb(cfg);

    std::vector<holoram::Complex32> truth(cfg.channels, holoram::Complex32{0.0f, 0.0f});
    truth[1] = holoram::Complex32{1.0f, 0.1f};
    truth[7] = holoram::Complex32{-0.35f, 0.25f};
    truth[11] = holoram::Complex32{0.2f, -0.1f};

    const auto y = holoram::synthesize_from_channel_values(cb, truth);

    auto matched = holoram::matched_filter_demix(y, cb);
    auto sic = holoram::sic_demix(y, cb, 1e-6f, 3);

    assert(matched.error == holoram::HoloError::Ok);
    assert(sic.error == holoram::HoloError::Ok);
    assert(sic.channel_values.size() == truth.size());
    assert(sic.residual_norm <= matched.residual_norm + 1e-3f);
    assert(sic.iterations == 3);

    return 0;
}
