#include "holoram/qh_codec.h"
#include <cassert>
#include <vector>

int main() {
    std::vector<std::uint8_t> input(4096);
    for (std::size_t i = 0; i < input.size(); ++i) input[i] = static_cast<std::uint8_t>(i & 0xFF);
    holoram::HoloCodebook cb({});
    holoram::HoloCodecConfig cfg{};
    cfg.codec_mode = holoram::HoloCodecMode::ExactRaw;
    auto enc = holoram::encode_dense(input.data(), static_cast<std::uint32_t>(input.size()), cfg, cb, 0);
    assert(enc.error == holoram::HoloError::Ok);
    auto dec = holoram::decode_dense(enc.payload.data(), static_cast<std::uint32_t>(enc.payload.size()), cfg, cb, 0);
    assert(dec.error == holoram::HoloError::Ok);
    assert(dec.decoded == input);
    return 0;
}
