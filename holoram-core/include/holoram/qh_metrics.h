#pragma once

#include <cstdint>

namespace holoram {

double byte_error_rate(const std::uint8_t* a, const std::uint8_t* b, std::uint32_t n);
double cosine_similarity(const float* a, const float* b, std::uint32_t n);
double psnr_u8(const std::uint8_t* a, const std::uint8_t* b, std::uint32_t n);
double ssim_placeholder(const std::uint8_t* a, const std::uint8_t* b, std::uint32_t n);

} // namespace holoram
