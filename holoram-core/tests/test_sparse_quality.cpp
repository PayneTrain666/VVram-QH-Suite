#include "holoram/qh_codec.h"
#include "holoram/qh_metrics.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
int main(){std::vector<std::uint8_t> input(2048); for(std::size_t i=0;i<input.size();++i) input[i]=static_cast<std::uint8_t>((i*3+(i/7))&0xFF); holoram::HoloCodebookConfig cb_cfg{}; cb_cfg.q=256; cb_cfg.channels=16; cb_cfg.strategy=holoram::HoloCodeStrategy::CazacZc; cb_cfg.fail_mu_max=0.75f; holoram::HoloCodebook cb(cb_cfg); holoram::HoloCodecConfig cfg{}; cfg.q=256; cfg.k_sparse=32; cfg.codec_mode=holoram::HoloCodecMode::ApproxVisual; cfg.quant_mode=holoram::HoloQuantMode::Int16Complex; auto enc=holoram::encode_sparse_topk(input.data(),static_cast<std::uint32_t>(input.size()),cfg,cb,3); assert(enc.error==holoram::HoloError::Ok); assert(!enc.payload.empty()); auto dec=holoram::decode_sparse_topk(enc.payload.data(),static_cast<std::uint32_t>(enc.payload.size()),cfg,cb,3); assert(dec.error==holoram::HoloError::Ok); assert(dec.decoded.size()==input.size()); const double ber=holoram::byte_error_rate(input.data(),dec.decoded.data(),static_cast<std::uint32_t>(input.size())); const double psnr=holoram::psnr_u8(input.data(),dec.decoded.data(),static_cast<std::uint32_t>(input.size())); assert(ber>=0.0&&ber<=1.0); assert(std::isfinite(psnr)); std::cout<<"sparse approx BER="<<ber<<" PSNR="<<psnr<<"\n"; return 0;}
