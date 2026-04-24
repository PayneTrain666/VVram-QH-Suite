#include "holoram/qh_metrics.h"

#include <cstdint>

namespace holoram {

namespace {

std::uint32_t update_crc32_byte(std::uint32_t crc, std::uint8_t byte) {
    crc ^= byte;
    for (int i = 0; i < 8; ++i) {
        const std::uint32_t mask = static_cast<std::uint32_t>(-(crc & 1u));
        crc = (crc >> 1u) ^ (0xEDB88320u & mask);
    }
    return crc;
}

} // namespace

std::uint32_t crc32_extend(std::uint32_t previous_crc, const void* data, std::uint32_t bytes) {
    const auto* p = static_cast<const std::uint8_t*>(data);
    std::uint32_t crc = ~previous_crc;

    for (std::uint32_t i = 0; i < bytes; ++i) {
        crc = update_crc32_byte(crc, p[i]);
    }

    return ~crc;
}

std::uint32_t crc32_bytes(const void* data, std::uint32_t bytes) {
    if (!data && bytes > 0) {
        return 0u;
    }
    return crc32_extend(0u, data, bytes);
}

} // namespace holoram
