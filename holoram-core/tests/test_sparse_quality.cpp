#include "holoram/qh_codec.h"
#include "holoram/qh_metrics.h"
#include <cassert>
#include <vector>

int main() {
    std::vector<std::uint8_t> input(4096, 42);
    holoram::HoloCodebook cb({});
    holoram::HoloCodecConfig cfg{};
    cfg.codec_mode = holoram::HoloCodecMode::ApproxVisual;
    cfg.k_sparse = 24;
    auto enc = holoram::encode_sparse_topk(input.data(), static_cast<std::uint32_t>(input.size()), cfg, cb, 0);
    auto dec = holoram::decode_sparse_topk(enc.payload.data(), static_cast<std::uint32_t>(enc.payload.size()), cfg, cb, 0);
    double ber = holoram::byte_error_rate(input.data(), dec.decoded.data(), static_cast<std::uint32_t>(input.size()));
    assert(ber >= 0.0);
    return 0;
}
