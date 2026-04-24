#include "holoram/types.h"

#include <cstdint>
#include <vector>

namespace holoram {

std::vector<std::uint8_t> compute_xor_residual(
    const std::uint8_t* original,
    const std::uint8_t* reconstructed,
    std::uint32_t n)
{
    std::vector<std::uint8_t> residual(n);
    for (std::uint32_t i = 0; i < n; ++i) {
        residual[i] = static_cast<std::uint8_t>(original[i] ^ reconstructed[i]);
    }
    return residual;
}

std::vector<std::uint8_t> apply_xor_residual(
    const std::uint8_t* reconstructed,
    const std::uint8_t* residual,
    std::uint32_t n)
{
    std::vector<std::uint8_t> exact(n);
    for (std::uint32_t i = 0; i < n; ++i) {
        exact[i] = static_cast<std::uint8_t>(reconstructed[i] ^ residual[i]);
    }
    return exact;
}

std::vector<std::uint8_t> rle_compress_residual(const std::vector<std::uint8_t>& in) {
    std::vector<std::uint8_t> out;
    out.reserve(in.size());

    std::size_t i = 0;
    while (i < in.size()) {
        const std::uint8_t value = in[i];
        std::size_t run = 1;
        while (i + run < in.size() && in[i + run] == value && run < 255) {
            ++run;
        }
        out.push_back(static_cast<std::uint8_t>(run));
        out.push_back(value);
        i += run;
    }

    return out;
}

bool rle_decompress_residual(
    const std::uint8_t* payload,
    std::uint32_t payload_bytes,
    std::uint32_t expected_bytes,
    std::vector<std::uint8_t>& out)
{
    out.clear();
    out.reserve(expected_bytes);

    std::uint32_t offset = 0;
    while (offset < payload_bytes) {
        if (offset + 2 > payload_bytes) {
            return false;
        }

        const std::uint8_t run = payload[offset++];
        const std::uint8_t value = payload[offset++];

        if (run == 0) {
            return false;
        }

        if (out.size() + run > expected_bytes) {
            return false;
        }

        out.insert(out.end(), run, value);
    }

    return out.size() == expected_bytes;
}

std::vector<std::uint8_t> maybe_compress_residual(
    const std::vector<std::uint8_t>& residual,
    HoloResidualCompression requested,
    HoloResidualCompression& used)
{
    if (requested == HoloResidualCompression::None) {
        used = HoloResidualCompression::None;
        return residual;
    }

    const auto rle = rle_compress_residual(residual);

    if (requested == HoloResidualCompression::Rle) {
        used = HoloResidualCompression::Rle;
        return rle;
    }

    if (rle.size() < residual.size()) {
        used = HoloResidualCompression::Rle;
        return rle;
    }

    used = HoloResidualCompression::None;
    return residual;
}

bool decompress_residual_payload(
    const std::uint8_t* payload,
    std::uint32_t payload_bytes,
    std::uint32_t expected_bytes,
    HoloResidualCompression compression,
    std::vector<std::uint8_t>& out)
{
    if (compression == HoloResidualCompression::None) {
        if (payload_bytes != expected_bytes) {
            return false;
        }
        out.assign(payload, payload + payload_bytes);
        return true;
    }

    if (compression == HoloResidualCompression::Rle) {
        return rle_decompress_residual(payload, payload_bytes, expected_bytes, out);
    }

    return false;
}

} // namespace holoram
