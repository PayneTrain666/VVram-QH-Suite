#include "holoram/qh_metrics.h"
#include <cmath>

namespace holoram {

double psnr_u8(const std::uint8_t* a, const std::uint8_t* b, std::uint32_t n) {
    if (n == 0) return 0.0;
    double mse = 0.0;
    for (std::uint32_t i = 0; i < n; ++i) { double d = static_cast<double>(a[i]) - static_cast<double>(b[i]); mse += d * d; }
    mse /= static_cast<double>(n);
    if (mse <= 1e-30) return 999.0;
    return 20.0 * std::log10(255.0 / std::sqrt(mse));
}

} // namespace holoram
