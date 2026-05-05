#include "holoram/qh_codec.h"

#include <sstream>

namespace holoram {

HoloPayloadValidation validate_sparse_payload_header(const HoloSparsePayloadHeader& h, std::uint32_t payload_bytes, std::uint32_t expected_q, std::uint32_t expected_channels) {
    if (h.magic != HOLO_SPARSE_PAYLOAD_MAGIC) return {HoloError::DecodeFailed, "bad sparse payload magic"};
    if (h.version != 2) return {HoloError::DecodeFailed, "unsupported sparse payload version"};
    if (h.struct_size != sizeof(HoloSparsePayloadHeader)) return {HoloError::DecodeFailed, "bad sparse payload struct_size"};
    if (payload_bytes < sizeof(HoloSparsePayloadHeader)) return {HoloError::DecodeFailed, "payload shorter than sparse header"};
    if (h.q == 0 || h.block_count == 0) return {HoloError::DecodeFailed, "zero q or block_count"};
    if (expected_q != 0 && h.q != expected_q) return {HoloError::InvalidArgument, "q mismatch"};
    if (h.k == 0 || h.k > HOLO_SPARSE_K_MAX || h.k > h.q) return {HoloError::DecodeFailed, "invalid sparse k"};
    if (h.quant_bits != 8 && h.quant_bits != 16) return {HoloError::UnsupportedMode, "unsupported quant_bits"};
    if (expected_channels != 0 && h.channel >= expected_channels) return {HoloError::InvalidArgument, "channel out of range"};
    const std::uint64_t sparse_bytes = static_cast<std::uint64_t>(h.block_count) * static_cast<std::uint64_t>(sizeof(HoloSparseBlock));
    const std::uint64_t min_bytes = static_cast<std::uint64_t>(sizeof(HoloSparsePayloadHeader)) + sparse_bytes;
    if (min_bytes > payload_bytes) return {HoloError::DecodeFailed, "sparse payload truncated"};
    return {HoloError::Ok, "ok"};
}

HoloPayloadValidation validate_residual_payload_header(const HoloResidualPayloadHeader& h, std::uint32_t available_bytes, std::uint32_t expected_decoded_bytes) {
    if (h.magic != HOLO_RESIDUAL_MAGIC) return {HoloError::DecodeFailed, "bad residual payload magic"};
    if (h.version != 2) return {HoloError::DecodeFailed, "unsupported residual payload version"};
    if (h.struct_size != sizeof(HoloResidualPayloadHeader)) return {HoloError::DecodeFailed, "bad residual payload struct_size"};
    if (available_bytes < sizeof(HoloResidualPayloadHeader)) return {HoloError::DecodeFailed, "available bytes shorter than residual header"};
    if (expected_decoded_bytes != 0 && h.decoded_bytes != expected_decoded_bytes) return {HoloError::DecodeFailed, "decoded_bytes mismatch"};
    if (h.residual_uncompressed_bytes != h.decoded_bytes) return {HoloError::DecodeFailed, "residual uncompressed count mismatch"};
    const auto compression = static_cast<HoloResidualCompression>(h.compression);
    if (compression != HoloResidualCompression::None && compression != HoloResidualCompression::Rle) return {HoloError::UnsupportedMode, "unsupported residual compression"};
    const std::uint64_t required = static_cast<std::uint64_t>(sizeof(HoloResidualPayloadHeader)) + static_cast<std::uint64_t>(h.residual_payload_bytes);
    if (required > available_bytes) return {HoloError::DecodeFailed, "residual payload truncated"};
    return {HoloError::Ok, "ok"};
}

std::string sparse_payload_header_to_string(const HoloSparsePayloadHeader& h) {
    std::ostringstream oss;
    oss << "HoloSparsePayloadHeader{version=" << h.version << ", struct_size=" << h.struct_size << ", magic=0x" << std::hex << h.magic << std::dec << ", decoded_bytes=" << h.decoded_bytes << ", q=" << h.q << ", block_count=" << h.block_count << ", k=" << h.k << ", quant_bits=" << h.quant_bits << ", channel=" << h.channel << ", decoded_crc32=0x" << std::hex << h.decoded_crc32 << ", sparse_crc32=0x" << h.sparse_crc32 << std::dec << "}";
    return oss.str();
}

std::string residual_payload_header_to_string(const HoloResidualPayloadHeader& h) {
    std::ostringstream oss;
    oss << "HoloResidualPayloadHeader{version=" << h.version << ", struct_size=" << h.struct_size << ", magic=0x" << std::hex << h.magic << std::dec << ", residual_payload_bytes=" << h.residual_payload_bytes << ", residual_uncompressed_bytes=" << h.residual_uncompressed_bytes << ", decoded_bytes=" << h.decoded_bytes << ", compression=" << h.compression << ", residual_payload_crc32=0x" << std::hex << h.residual_payload_crc32 << ", decoded_crc32=0x" << h.decoded_crc32 << std::dec << "}";
    return oss.str();
}

std::vector<Complex32> build_channel_observation(const HoloSparseBlock& block, std::uint32_t q) {
    std::vector<Complex32> observation(q, Complex32{0.0f, 0.0f});
    if (block.magic != HOLO_SPARSE_BLOCK_MAGIC || block.version != 1 || block.struct_size != sizeof(HoloSparseBlock) || block.q != q || block.k == 0 || block.k > HOLO_SPARSE_K_MAX) return observation;
    for (std::uint16_t i = 0; i < block.k; ++i) {
        if (block.idx[i] < q) observation[block.idx[i]] = Complex32{static_cast<float>(block.re[i]) * block.scale, static_cast<float>(block.im[i]) * block.scale};
    }
    return observation;
}

HoloDemixResult demix_sparse_observation(const std::vector<Complex32>& observation, const HoloCodebook& codebook, const HoloCodecConfig& cfg) {
    HoloDemixConfig dc{};
    dc.mode = cfg.demix_mode;
    dc.lambda = cfg.lambda;
    dc.sic_iters = cfg.sic_iters;
    dc.use_gram_cache = true;
    return demix_channels(observation, codebook, dc);
}

HoloSparseBlockDemixResult decode_sparse_block_with_demix(const HoloSparseBlock& block, const HoloCodebook& codebook, const HoloCodecConfig& cfg, HoloChannelId target_channel) {
    HoloSparseBlockDemixResult out{};
    const auto q = codebook.config().q;
    if (target_channel >= codebook.config().channels) { out.error = HoloError::InvalidArgument; return out; }
    out.observation = build_channel_observation(block, q);
    out.demix = demix_sparse_observation(out.observation, codebook, cfg);
    if (out.demix.error != HoloError::Ok) { out.error = out.demix.error; return out; }
    out.decoded_coefficients.assign(q, Complex32{0.0f, 0.0f});
    const auto value = out.demix.channel_values[target_channel];
    const auto* code = codebook.channel(target_channel);
    for (std::uint32_t k = 0; k < q; ++k) out.decoded_coefficients[k] = value * std::conj(code[k]);
    out.error = HoloError::Ok;
    return out;
}

} // namespace holoram
