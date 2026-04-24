#include "holoram/qh_codec.h"
#include "holoram/formats/holo_sparse_block.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

int main() {
    std::vector<std::uint8_t> input(512);
    for (std::size_t i = 0; i < input.size(); ++i) {
        input[i] = static_cast<std::uint8_t>(i & 0xFF);
    }

    holoram::HoloCodebookConfig cb_cfg{};
    cb_cfg.q = 256;
    cb_cfg.channels = 8;
    cb_cfg.strategy = holoram::HoloCodeStrategy::CazacZc;
    cb_cfg.fail_mu_max = 0.75f;
    holoram::HoloCodebook cb(cb_cfg);

    holoram::HoloCodecConfig cfg{};
    cfg.q = 256;
    cfg.k_sparse = 16;
    cfg.codec_mode = holoram::HoloCodecMode::ApproxVector;
    cfg.quant_mode = holoram::HoloQuantMode::Int8Complex;
    cfg.enable_crc = true;

    auto enc = holoram::encode_sparse_topk(input.data(), static_cast<std::uint32_t>(input.size()), cfg, cb, 2);

    assert(enc.error == holoram::HoloError::Ok);
    assert(enc.payload.size() >= sizeof(holoram::HoloSparsePayloadHeader));

    holoram::HoloSparsePayloadHeader header{};
    std::memcpy(&header, enc.payload.data(), sizeof(header));

    assert(header.magic == holoram::HOLO_SPARSE_PAYLOAD_MAGIC);
    assert(header.version == 2);
    assert(header.decoded_bytes == input.size());
    assert(header.q == 256);
    assert(header.k == 16);
    assert(header.quant_bits == 8);
    assert(header.block_count == 2);
    assert(header.channel == 2);
    assert(header.sparse_crc32 != 0);

    return 0;
}
