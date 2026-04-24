#include "holoram/qh_codec.h"
#include "holoram/qh_metrics.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace holoram {

std::int8_t clamp_i8(int v);
std::int16_t clamp_i16(int v);
float choose_i8_scale(float max_abs);
float choose_i16_scale(float max_abs);

std::vector<std::uint8_t> compute_xor_residual(const std::uint8_t*, const std::uint8_t*, std::uint32_t);
std::vector<std::uint8_t> apply_xor_residual(const std::uint8_t*, const std::uint8_t*, std::uint32_t);
std::vector<std::uint8_t> maybe_compress_residual(const std::vector<std::uint8_t>&, HoloResidualCompression, HoloResidualCompression&);
bool decompress_residual_payload(const std::uint8_t*, std::uint32_t, std::uint32_t, HoloResidualCompression, std::vector<std::uint8_t>&);

#include "sparse_topk_codec_helpers.inc"
#include "sparse_topk_codec_api.inc"

} // namespace holoram
