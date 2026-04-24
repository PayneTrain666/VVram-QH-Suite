#pragma once
#include "holoram/errors.h"
#include "holoram/formats/holo_block.h"
#include "holoram/formats/holo_sparse_block.h"
#include "holoram/qh_codebook.h"
#include "holoram/types.h"
#include <cstdint>
#include <vector>
namespace holoram {
struct HoloCodecConfig {
    std::uint32_t q = 256;
    std::uint32_t k_sparse = 24;
    HoloCodecMode codec_mode = HoloCodecMode::ApproxVisual;
    HoloQuantMode quant_mode = HoloQuantMode::Int16Complex;
    HoloDemixMode demix_mode = HoloDemixMode::Matched;
    float lambda = 1e-2f;
    std::uint32_t sic_iters = 2;
};
struct HoloEncodeResult { HoloError error = HoloError::Ok; std::vector<std::uint8_t> payload; HoloBlockHeader header{}; };
struct HoloDecodeResult { HoloError error = HoloError::Ok; std::vector<std::uint8_t> decoded; };
HoloEncodeResult encode_dense(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloDecodeResult decode_dense(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloEncodeResult encode_sparse_topk(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
HoloDecodeResult decode_sparse_topk(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig& cfg, const HoloCodebook& codebook, HoloChannelId channel);
}
