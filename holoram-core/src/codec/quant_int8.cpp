#include <algorithm>
#include <cmath>
#include <cstdint>

namespace holoram {

std::int8_t clamp_i8(int v) {
    return static_cast<std::int8_t>(std::max(-127, std::min(127, v)));
}

float choose_i8_scale(float max_abs) {
    return max_abs <= 1e-12f ? 1.0f : max_abs / 127.0f;
}

} // namespace holoram
