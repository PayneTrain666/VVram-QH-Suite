#include "holoram/qh_codec.h"

#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    std::vector<std::uint8_t> input(1024);
    for (std::size_t i = 0; i < input.size(); ++i) {
        input[i] = static_cast<std::uint8_t>((i * 31 + 7) & 0xFF);
    }

    holoram::HoloCodebookConfig cb_cfg{};
    cb_cfg.q = 256;
    cb_cfg.channels = 8;
    cb_cfg.strategy = holoram::HoloCodeStrategy::MubDft;
    cb_cfg.fail_mu_max = 0.75f;
    holoram::HoloCodebook cb(cb_cfg);

    holoram::HoloCodecConfig cfg{};
    cfg.q = 256;
    cfg.k_sparse = 24;
    cfg.codec_mode = holoram::HoloCodecMode::ExactQhResidual;
    cfg.quant_mode = holoram::HoloQuantMode::Int16Complex;
    cfg.enable_crc = true;

    auto enc = holoram::encode_sparse_topk(input.data(), static_cast<std::uint32_t>(input.size()), cfg, cb, 2);
    assert(enc.error == holoram::HoloError::Ok);
    assert(!enc.payload.empty());

    auto corrupted = enc.payload;
    corrupted[sizeof(holoram::HoloSparsePayloadHeader) + 16] ^= 0x55u;

    auto dec = holoram::decode_sparse_topk(corrupted.data(), static_cast<std::uint32_t>(corrupted.size()), cfg, cb, 2);
    assert(dec.error == holoram::HoloError::IntegrityCheckFailed);

    return 0;
}
