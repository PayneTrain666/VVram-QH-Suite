#include "holoram/qh_codebook.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

namespace {

void check_codebook(holoram::HoloCodeStrategy strategy) {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = 256;
    cfg.channels = 16;
    cfg.strategy = strategy;
    cfg.seed = 1234;
    cfg.salt = 5678;
    cfg.strict_validation = true;
    cfg.fail_mu_max = 0.75f;

    holoram::HoloCodebook cb(cfg);
    assert(!cb.empty());
    assert(cb.codes().size() == static_cast<std::size_t>(cfg.q) * cfg.channels);

    for (std::uint32_t c = 0; c < cfg.channels; ++c) {
        const auto* ch = cb.channel(c);
        for (std::uint32_t k = 0; k < cfg.q; ++k) {
            const float mag = std::abs(ch[k]);
            assert(std::isfinite(ch[k].real()));
            assert(std::isfinite(ch[k].imag()));
            assert(std::abs(mag - 1.0f) < 2.5e-3f);
        }
    }

    auto report = cb.measure_coherence();
    assert(report.pair_count == (cfg.channels * (cfg.channels - 1)) / 2);
    assert(report.mu_max >= 0.0f);
    assert(report.mu_avg >= 0.0f);
    assert(report.mu_rms >= 0.0f);
    assert(report.mu_max <= 1.0f + 1e-3f);

    auto validation = cb.validate();
    assert(validation.dimensions_ok);
    assert(validation.unit_magnitude_ok);
    assert(validation.finite_ok);
    assert(validation.quality != holoram::HoloCodebookQuality::Failed);

    std::cout << "strategy=" << static_cast<int>(strategy)
              << " mu_max=" << report.mu_max
              << " mu_avg=" << report.mu_avg
              << " mu_rms=" << report.mu_rms
              << " quality=" << holoram::to_string(validation.quality)
              << "\n";
}

} // namespace

int main() {
    check_codebook(holoram::HoloCodeStrategy::CazacZc);
    check_codebook(holoram::HoloCodeStrategy::MubDft);
    return 0;
}
