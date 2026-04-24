#pragma once
#include <cstdint>
namespace holoram {
constexpr std::uint32_t HOLO_SPARSE_BLOCK_MAGIC = 0x48535042u;
constexpr std::uint32_t HOLO_SPARSE_PAYLOAD_MAGIC = 0x48535048u;
constexpr std::uint32_t HOLO_RESIDUAL_MAGIC = 0x48524553u;
constexpr std::uint32_t HOLO_SPARSE_K_MAX = 64;
struct HoloSparsePayloadHeader {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloSparsePayloadHeader);
    std::uint32_t magic = HOLO_SPARSE_PAYLOAD_MAGIC;
    std::uint32_t decoded_bytes = 0;
    std::uint32_t q = 256;
    std::uint32_t block_count = 0;
    std::uint16_t k = 24;
    std::uint16_t quant_bits = 16;
    std::uint16_t codec_mode = 0;
    std::uint16_t quant_mode = 0;
    std::uint16_t demix_mode = 0;
    std::uint16_t channel = 0;
    std::uint32_t flags = 0;
};
struct HoloSparseBlock {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloSparseBlock);
    std::uint32_t magic = HOLO_SPARSE_BLOCK_MAGIC;
    std::uint16_t q = 256;
    std::uint16_t k = 24;
    std::uint16_t qbits = 16;
    std::uint16_t flags = 0;
    float scale = 1.0f;
    std::uint16_t idx[HOLO_SPARSE_K_MAX]{};
    std::int16_t re[HOLO_SPARSE_K_MAX]{};
    std::int16_t im[HOLO_SPARSE_K_MAX]{};
};
struct HoloResidualPayloadHeader {
    std::uint32_t version = 1;
    std::uint32_t struct_size = sizeof(HoloResidualPayloadHeader);
    std::uint32_t magic = HOLO_RESIDUAL_MAGIC;
    std::uint32_t residual_bytes = 0;
    std::uint32_t decoded_bytes = 0;
    std::uint32_t flags = 0;
};
}
