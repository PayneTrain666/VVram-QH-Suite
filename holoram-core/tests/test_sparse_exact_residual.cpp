#include "holoram/qh_codec.h"
#include <cassert>
#include <cstdint>
#include <vector>
int main(){std::vector<std::uint8_t> input(1537); for(std::size_t i=0;i<input.size();++i) input[i]=static_cast<std::uint8_t>((i*19+23)&0xFF); holoram::HoloCodebookConfig cb_cfg{}; cb_cfg.q=256; cb_cfg.channels=16; cb_cfg.strategy=holoram::HoloCodeStrategy::MubDft; cb_cfg.seed=44; cb_cfg.salt=99; cb_cfg.fail_mu_max=0.75f; holoram::HoloCodebook cb(cb_cfg); holoram::HoloCodecConfig cfg{}; cfg.q=256; cfg.k_sparse=24; cfg.codec_mode=holoram::HoloCodecMode::ExactQhResidual; cfg.quant_mode=holoram::HoloQuantMode::Int16Complex; auto enc=holoram::encode_sparse_topk(input.data(),static_cast<std::uint32_t>(input.size()),cfg,cb,5); assert(enc.error==holoram::HoloError::Ok); auto dec=holoram::decode_sparse_topk(enc.payload.data(),static_cast<std::uint32_t>(enc.payload.size()),cfg,cb,5); assert(dec.error==holoram::HoloError::Ok); assert(dec.decoded==input); return 0;}
