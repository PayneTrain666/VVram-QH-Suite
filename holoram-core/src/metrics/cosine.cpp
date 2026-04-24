#include "holoram/qh_metrics.h"
#include <cmath>

namespace holoram {

double cosine_similarity(const float* a, const float* b, std::uint32_t n) {
    double dot = 0.0, aa = 0.0, bb = 0.0;
    for (std::uint32_t i = 0; i < n; ++i) { dot += static_cast<double>(a[i]) * b[i]; aa += static_cast<double>(a[i]) * a[i]; bb += static_cast<double>(b[i]) * b[i]; }
    if (aa <= 1e-30 || bb <= 1e-30) return 0.0;
    return dot / (std::sqrt(aa) * std::sqrt(bb));
}

} // namespace holoram
