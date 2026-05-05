#pragma once

#include "holoram/errors.h"
#include "holoram/formats/holo_block.h"
#include "holoram/formats/holo_sparse_block.h"
#include "holoram/qh_codebook.h"
#include "holoram/qh_demix.h"
#include "holoram/types.h"

#include <cstdint>
#include <string>
#include <vector>

namespace holoram {

struct HoloCodecConfig {
    std::uint32_t q = 256;
    std::uint32_t k_sparse = 24;
    HoloCodecMode codec_mode = HoloCodecMode::ApproxVisual;
    HoloQuantMode quant_mode = HoloQuantMode::Int16Complex;
    HoloDemixMode demix_mode = HoloDemixMode::Matched;
    HoloResidualCompression residual_compression = HoloResidualCompression::AutoRle;
    float lambda = 1e-2f;
    std::uint32_t sic_iters = 2;
    bool enable_crc = true;
};

struct HoloEncodeResult { HoloError error = HoloError::Ok; std::vector<std::uint8_t> payload; HoloBlockHeader header{}; };
struct HoloDecodeResult { HoloError error = HoloError::Ok; std::vector<std::uint8_t> decoded; };
struct HoloPayloadValidation { HoloError error = HoloError::Ok; std::string message; };
struct HoloSparseBlockDemixResult { HoloError error = HoloError::Ok; std::vector<Complex32> observation; HoloDemixResult demix; std::vector<Complex32> decoded_coefficients; };

HoloEncodeResult encode_dense(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloDecodeResult decode_dense(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloEncodeResult encode_sparse_topk(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloDecodeResult decode_sparse_topk(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);

HoloPayloadValidation validate_sparse_payload_header(const HoloSparsePayloadHeader& header, std::uint32_t payload_bytes, std::uint32_t expected_q = 0, std::uint32_t expected_channels = 0);
HoloPayloadValidation validate_residual_payload_header(const HoloResidualPayloadHeader& header, std::uint32_t available_bytes, std::uint32_t expected_decoded_bytes = 0);
std::string sparse_payload_header_to_string(const HoloSparsePayloadHeader& header);
std::string residual_payload_header_to_string(const HoloResidualPayloadHeader& header);

std::vector<Complex32> build_channel_observation(const HoloSparseBlock& block, std::uint32_t q);
HoloDemixResult demix_sparse_observation(const std::vector<Complex32>& observation, const HoloCodebook& codebook, const HoloCodecConfig& cfg);
HoloSparseBlockDemixResult decode_sparse_block_with_demix(const HoloSparseBlock& block, const HoloCodebook& codebook, const HoloCodecConfig& cfg, HoloChannelId target_channel);

} // namespace holoram
