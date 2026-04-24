#include "holoram/qh_codec.h"
#include <cstring>

namespace holoram {

HoloEncodeResult encode_sparse_topk(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook&, HoloChannelId) {
    HoloEncodeResult out{};
    out.header.decoded_bytes = decoded_bytes;
    out.header.codec_mode = static_cast<std::uint16_t>(cfg.codec_mode);
    out.header.quant_mode = static_cast<std::uint16_t>(cfg.quant_mode);
    out.header.demix_mode = static_cast<std::uint16_t>(cfg.demix_mode);
    // v0.0.1 placeholder: sparse Top-K spectral payload will replace this passthrough.
    out.payload.resize(decoded_bytes);
    std::memcpy(out.payload.data(), decoded, decoded_bytes);
    out.header.payload_bytes = decoded_bytes;
    return out;
}

HoloDecodeResult decode_sparse_topk(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig&, const HoloCodebook&, HoloChannelId) {
    HoloDecodeResult out{};
    out.decoded.resize(payload_bytes);
    std::memcpy(out.decoded.data(), payload, payload_bytes);
    return out;
}

} // namespace holoram
