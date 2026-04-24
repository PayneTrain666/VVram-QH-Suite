#include <algorithm>
#include <cstdint>
namespace holoram { std::int16_t clamp_i16(int v){return static_cast<std::int16_t>(std::max(-32767,std::min(32767,v)));} float choose_i16_scale(float max_abs){return max_abs<=1e-12f?1.0f:max_abs/32767.0f;} }
