#include "holoram/types.h"
#include <vector>

namespace holoram {

std::vector<Complex32> matched_filter_demix_placeholder(const std::vector<Complex32>& y, const std::vector<Complex32>& code) {
    std::vector<Complex32> out(y.size());
    for (std::size_t i = 0; i < y.size(); ++i) out[i] = std::conj(code[i]) * y[i];
    return out;
}

} // namespace holoram
