#include "holoram/qh_codec.h"
#include <cstring>
namespace holoram {
HoloEncodeResult encode_dense(const void* decoded,std::uint32_t decoded_bytes,const HoloCodecConfig& cfg,const HoloCodebook&,HoloChannelId){HoloEncodeResult out{}; if(!decoded&&decoded_bytes>0){out.error=HoloError::InvalidArgument;return out;} out.header.decoded_bytes=decoded_bytes; out.header.codec_mode=static_cast<std::uint16_t>(cfg.codec_mode); out.header.quant_mode=static_cast<std::uint16_t>(cfg.quant_mode); out.header.demix_mode=static_cast<std::uint16_t>(cfg.demix_mode); out.payload.resize(decoded_bytes); if(decoded_bytes>0) std::memcpy(out.payload.data(),decoded,decoded_bytes); out.header.payload_bytes=decoded_bytes; out.error=HoloError::Ok; return out;}
HoloDecodeResult decode_dense(const void* payload,std::uint32_t payload_bytes,const HoloCodecConfig&,const HoloCodebook&,HoloChannelId){HoloDecodeResult out{}; if(!payload&&payload_bytes>0){out.error=HoloError::InvalidArgument;return out;} out.decoded.resize(payload_bytes); if(payload_bytes>0) std::memcpy(out.decoded.data(),payload,payload_bytes); out.error=HoloError::Ok; return out;}
}
