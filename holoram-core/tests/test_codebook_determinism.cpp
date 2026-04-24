#include "holoram/qh_codebook.h"

#include <cassert>
#include <cmath>
#include <cstdint>

namespace {

bool same_codebook(const holoram::HoloCodebook& a, const holoram::HoloCodebook& b) {
    const auto& ca = a.codes();
    const auto& cb = b.codes();
    if (ca.size() != cb.size()) return false;
    for (std::size_t i = 0; i < ca.size(); ++i) {
        if (std::abs(ca[i].real() - cb[i].real()) > 1e-7f) return false;
        if (std::abs(ca[i].imag() - cb[i].imag()) > 1e-7f) return false;
    }
    return true;
}

void check_determinism(holoram::HoloCodeStrategy strategy) {
    holoram::HoloCodebookConfig cfg{};
    cfg.q = 256;
    cfg.channels = 16;
    cfg.strategy = strategy;
    cfg.seed = 9876;
    cfg.salt = 123;
    cfg.fail_mu_max = 0.75f;

    holoram::HoloCodebook a(cfg);
    holoram::HoloCodebook b(cfg);
    assert(same_codebook(a, b));
    assert(a.validate().quality != holoram::HoloCodebookQuality::Failed);
    assert(b.validate().quality != holoram::HoloCodebookQuality::Failed);
}

} // namespace

int main() {
    check_determinism(holoram::HoloCodeStrategy::CazacZc);
    check_determinism(holoram::HoloCodeStrategy::MubDft);
    return 0;
}
