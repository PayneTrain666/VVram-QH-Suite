#include "holoram/qh_codebook.h"
#include "holoram/qh_metrics.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

const char* strategy_name(holoram::HoloCodeStrategy s) {
    switch (s) {
        case holoram::HoloCodeStrategy::CazacZc: return "CAZAC_ZC";
        case holoram::HoloCodeStrategy::MubDft: return "MUB_DFT";
        default: return "UNKNOWN";
    }
}

void run_one(std::uint32_t q, std::uint32_t channels, holoram::HoloCodeStrategy strategy) {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = q;
    cfg.channels = channels;
    cfg.strategy = strategy;
    cfg.seed = 1234;
    cfg.salt = 99;
    cfg.strict_validation = false;
    cfg.warn_mu_max = 0.20f;
    cfg.fail_mu_max = 0.75f;

    const auto t0 = holoram::now_ns();
    holoram::HoloCodebook cb(cfg);
    const auto t1 = holoram::now_ns();
    const auto report = cb.measure_coherence();
    const auto validation = cb.validate();
    const auto t2 = holoram::now_ns();

    const double build_ms = static_cast<double>(t1 - t0) / 1e6;
    const double coherence_ms = static_cast<double>(t2 - t1) / 1e6;

    std::cout
        << std::setw(8) << strategy_name(strategy)
        << " Q=" << std::setw(4) << q
        << " C=" << std::setw(2) << channels
        << " build_ms=" << std::setw(9) << std::fixed << std::setprecision(3) << build_ms
        << " coh_ms=" << std::setw(9) << std::fixed << std::setprecision(3) << coherence_ms
        << " mu_max=" << std::setw(9) << std::fixed << std::setprecision(6) << report.mu_max
        << " mu_avg=" << std::setw(9) << std::fixed << std::setprecision(6) << report.mu_avg
        << " mu_rms=" << std::setw(9) << std::fixed << std::setprecision(6) << report.mu_rms
        << " worst=(" << report.worst_i << "," << report.worst_j << ")"
        << " quality=" << holoram::to_string(validation.quality)
        << "\n";
}

} // namespace

int main() {
    const std::vector<std::uint32_t> q_values = {256, 512, 1024};
    const std::vector<std::uint32_t> c_values = {8, 16, 32};
    const std::vector<holoram::HoloCodeStrategy> strategies = {holoram::HoloCodeStrategy::CazacZc, holoram::HoloCodeStrategy::MubDft};

    std::cout << "HoloRAM codebook coherence benchmark\n";
    std::cout << "------------------------------------\n";

    for (const auto strategy : strategies) {
        for (const auto q : q_values) {
            for (const auto c : c_values) {
                try {
                    run_one(q, c, strategy);
                } catch (const std::exception& e) {
                    std::cerr << "FAILED strategy=" << strategy_name(strategy) << " Q=" << q << " C=" << c << " error=" << e.what() << "\n";
                }
            }
        }
    }

    return 0;
}
