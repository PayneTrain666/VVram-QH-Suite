#include "holoram/qh_codebook.h"

#include <cassert>
#include <cmath>
#include <cstdint>

namespace {

float average_abs_difference(const holoram::HoloCodebook& a, const holoram::HoloCodebook& b) {
    const auto& ca = a.codes();
    const auto& cb = b.codes();
    assert(ca.size() == cb.size());
    double acc = 0.0;
    for (std::size_t i = 0; i < ca.size(); ++i) acc += std::abs(ca[i] - cb[i]);
    return static_cast<float>(acc / static_cast<double>(ca.size()));
}

void check_salt_changes_output(holoram::HoloCodeStrategy strategy) {
    holoram::HoloCodebookConfig cfg_a{};
    cfg_a.q = 256;
    cfg_a.channels = 16;
    cfg_a.strategy = strategy;
    cfg_a.seed = 1111;
    cfg_a.salt = 1;
    cfg_a.fail_mu_max = 0.75f;
    auto cfg_b = cfg_a;
    cfg_b.salt = 2;
    holoram::HoloCodebook a(cfg_a);
    holoram::HoloCodebook b(cfg_b);
    const float diff = average_abs_difference(a, b);
    assert(diff > 1e-3f);
}

void check_seed_changes_output(holoram::HoloCodeStrategy strategy) {
    holoram::HoloCodebookConfig cfg_a{};
    cfg_a.q = 256;
    cfg_a.channels = 16;
    cfg_a.strategy = strategy;
    cfg_a.seed = 100;
    cfg_a.salt = 50;
    cfg_a.fail_mu_max = 0.75f;
    auto cfg_b = cfg_a;
    cfg_b.seed = 101;
    holoram::HoloCodebook a(cfg_a);
    holoram::HoloCodebook b(cfg_b);
    const float diff = average_abs_difference(a, b);
    assert(diff > 1e-3f);
}

} // namespace

int main() {
    check_salt_changes_output(holoram::HoloCodeStrategy::CazacZc);
    check_salt_changes_output(holoram::HoloCodeStrategy::MubDft);
    check_seed_changes_output(holoram::HoloCodeStrategy::CazacZc);
    check_seed_changes_output(holoram::HoloCodeStrategy::MubDft);
    return 0;
}
