#include "holoram/core.h"

#include <iomanip>
#include <iostream>
#include <vector>

namespace {

const char* mode_name(holoram::HoloDemixMode mode) {
    switch (mode) {
        case holoram::HoloDemixMode::Matched:
            return "matched";
        case holoram::HoloDemixMode::Mmse:
            return "mmse";
        case holoram::HoloDemixMode::MmseSic:
            return "mmse+sic";
        default:
            return "unknown";
    }
}

void run_one(std::uint32_t q, std::uint32_t channels, std::uint32_t k, holoram::HoloDemixMode mode) {
    (void)k;

    holoram::HoloCodebookConfig cb_cfg{};
    cb_cfg.q = q;
    cb_cfg.channels = channels;
    cb_cfg.strategy = holoram::HoloCodeStrategy::MubDft;
    cb_cfg.strict_validation = true;
    cb_cfg.fail_mu_max = 0.75f;

    holoram::HoloCodebook cb(cb_cfg);

    std::vector<holoram::Complex32> truth(channels, holoram::Complex32{0.0f, 0.0f});
    for (std::uint32_t c = 0; c < channels; ++c) {
        if ((c % 3) == 0) {
            truth[c] = holoram::Complex32{0.025f * static_cast<float>(c + 1), -0.01f * static_cast<float>(c)};
        }
    }

    const auto y = holoram::synthesize_from_channel_values(cb, truth);

    const auto t0 = holoram::now_ns();
    holoram::HoloDemixResult result{};

    if (mode == holoram::HoloDemixMode::Matched) {
        result = holoram::matched_filter_demix(y, cb);
    } else if (mode == holoram::HoloDemixMode::Mmse) {
        result = holoram::mmse_demix(y, cb, 1e-3f);
    } else {
        result = holoram::sic_demix(y, cb, 1e-3f, 2);
    }

    const auto t1 = holoram::now_ns();
    const double ms = static_cast<double>(t1 - t0) / 1e6;

    std::cout
        << "Q=" << std::setw(4) << q
        << " C=" << std::setw(2) << channels
        << " K=" << std::setw(2) << k
        << " mode=" << std::setw(8) << mode_name(mode)
        << " error=" << holoram::to_string(result.error)
        << " residual=" << std::setw(12) << std::fixed << std::setprecision(6) << result.residual_norm
        << " iters=" << result.iterations
        << " ms=" << std::setw(8) << std::fixed << std::setprecision(3) << ms
        << "\n";
}

} // namespace

int main() {
    std::cout << "HoloRAM demix benchmark\n";
    std::cout << "----------------------\n";

    const std::vector<std::uint32_t> q_values = {256, 512, 1024};
    const std::vector<std::uint32_t> c_values = {8, 16, 32};
    const std::vector<std::uint32_t> k_values = {8, 16, 24, 32, 48, 64};
    const std::vector<holoram::HoloDemixMode> modes = {holoram::HoloDemixMode::Matched, holoram::HoloDemixMode::Mmse, holoram::HoloDemixMode::MmseSic};

    for (auto q : q_values) {
        for (auto c : c_values) {
            for (auto k : k_values) {
                for (auto mode : modes) {
                    run_one(q, c, k, mode);
                }
            }
        }
    }

    return 0;
}
