#include "holoram/qh_metrics.h"

namespace holoram {

double byte_error_rate(const std::uint8_t* a, const std::uint8_t* b, std::uint32_t n) {
    if (n == 0) return 0.0;
    std::uint32_t bad = 0;
    for (std::uint32_t i = 0; i < n; ++i) if (a[i] != b[i]) ++bad;
    return static_cast<double>(bad) / static_cast<double>(n);
}

} // namespace holoram
