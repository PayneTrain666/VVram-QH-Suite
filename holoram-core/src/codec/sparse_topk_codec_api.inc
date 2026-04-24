HoloEncodeResult encode_sparse_topk(const void* decoded, std::uint32_t decoded_bytes, const HoloCodecConfig& cfg, const HoloCodebook& cb, HoloChannelId channel) {
    HoloEncodeResult out{};
    if (!decoded && decoded_bytes > 0) { out.error = HoloError::InvalidArgument; return out; }
    if (channel >= cb.config().channels) { out.error = HoloError::InvalidArgument; return out; }
    std::uint32_t q = 0;
    try { q = codec_q(cfg, cb); } catch (...) { out.error = HoloError::InvalidArgument; return out; }
    if (q > std::numeric_limits<std::uint16_t>::max()) { out.error = HoloError::UnsupportedMode; return out; }

    const auto* bytes = static_cast<const std::uint8_t*>(decoded);
    const std::uint16_t k = static_cast<std::uint16_t>(std::max<std::uint32_t>(1, std::min<std::uint32_t>(cfg.k_sparse, std::min<std::uint32_t>(q, HOLO_SPARSE_K_MAX))));
    const std::uint16_t qb = qbits(cfg.quant_mode);
    const std::uint32_t blocks = decoded_bytes == 0 ? 1u : ((decoded_bytes + q - 1u) / q);

    HoloSparsePayloadHeader h{};
    h.decoded_bytes = decoded_bytes; h.q = q; h.block_count = blocks; h.k = k; h.quant_bits = qb;
    h.codec_mode = static_cast<std::uint16_t>(cfg.codec_mode);
    h.quant_mode = static_cast<std::uint16_t>(cfg.quant_mode);
    h.demix_mode = static_cast<std::uint16_t>(cfg.demix_mode);
    h.channel = channel;
    h.residual_compression = static_cast<std::uint16_t>(cfg.residual_compression);
    if (cfg.enable_crc) h.decoded_crc32 = crc32_bytes(decoded, decoded_bytes);

    append_pod(out.payload, h);
    const std::uint32_t sparse_start = static_cast<std::uint32_t>(out.payload.size());
    const auto* code = cb.channel(channel);

    for (std::uint32_t b = 0; b < blocks; ++b) {
        auto coeff = dft(make_block(bytes, decoded_bytes, b, q));
        for (std::uint32_t i = 0; i < q; ++i) coeff[i] *= code[i];
        HoloSparseBlock sb{}; pack_topk(sb, coeff, q, k, qb); append_pod(out.payload, sb);
    }

    if (cfg.enable_crc) {
        h.sparse_crc32 = crc32_bytes(out.payload.data() + sparse_start, static_cast<std::uint32_t>(out.payload.size()) - sparse_start);
        std::memcpy(out.payload.data(), &h, sizeof(h));
    }

    if (cfg.codec_mode == HoloCodecMode::ExactQhResidual) {
        HoloError re = HoloError::Ok;
        auto approx = sparse_reconstruct(out.payload.data(), static_cast<std::uint32_t>(out.payload.size()), cb, re, nullptr);
        if (re != HoloError::Ok || approx.size() != decoded_bytes) { out.error = HoloError::EncodeFailed; return out; }
        const auto residual = compute_xor_residual(bytes, approx.data(), decoded_bytes);
        HoloResidualCompression used = HoloResidualCompression::None;
        const auto residual_payload = maybe_compress_residual(residual, cfg.residual_compression, used);
        HoloResidualPayloadHeader rh{};
        rh.residual_payload_bytes = static_cast<std::uint32_t>(residual_payload.size());
        rh.residual_uncompressed_bytes = static_cast<std::uint32_t>(residual.size());
        rh.decoded_bytes = decoded_bytes;
        rh.compression = static_cast<std::uint16_t>(used);
        if (cfg.enable_crc) {
            rh.residual_payload_crc32 = crc32_bytes(residual_payload.data(), static_cast<std::uint32_t>(residual_payload.size()));
            rh.decoded_crc32 = h.decoded_crc32;
        }
        append_pod(out.payload, rh);
        out.payload.insert(out.payload.end(), residual_payload.begin(), residual_payload.end());
    }

    out.header.decoded_bytes = decoded_bytes;
    out.header.payload_bytes = static_cast<std::uint32_t>(out.payload.size());
    out.header.q = q;
    out.header.channels = cb.config().channels;
    out.header.codec_mode = static_cast<std::uint16_t>(cfg.codec_mode);
    out.header.quant_mode = static_cast<std::uint16_t>(cfg.quant_mode);
    out.header.demix_mode = static_cast<std::uint16_t>(cfg.demix_mode);
    out.header.residual_compression = h.residual_compression;
    out.header.decoded_crc32 = h.decoded_crc32;
    out.header.payload_crc32 = cfg.enable_crc ? crc32_bytes(out.payload.data(), static_cast<std::uint32_t>(out.payload.size())) : 0u;
    out.error = HoloError::Ok;
    return out;
}

HoloDecodeResult decode_sparse_topk(const void* payload, std::uint32_t payload_bytes, const HoloCodecConfig& cfg, const HoloCodebook& cb, HoloChannelId) {
    HoloDecodeResult out{};
    if (!payload && payload_bytes > 0) { out.error = HoloError::InvalidArgument; return out; }

    const auto* bytes = static_cast<const std::uint8_t*>(payload);
    HoloSparsePayloadHeader sh{};
    if (payload_bytes < sizeof(sh)) { out.error = HoloError::DecodeFailed; return out; }
    std::memcpy(&sh, bytes, sizeof(sh));
    if (!sparse_header_ok(sh)) { out.error = HoloError::DecodeFailed; return out; }

    HoloError err = HoloError::Ok;
    std::uint32_t sparse_end = 0;
    auto approx = sparse_reconstruct(bytes, payload_bytes, cb, err, &sparse_end);
    if (err != HoloError::Ok) { out.error = err; return out; }

    if (cfg.codec_mode != HoloCodecMode::ExactQhResidual) {
        out.decoded = std::move(approx);
        out.error = HoloError::Ok;
        return out;
    }

    HoloResidualPayloadHeader rh{};
    if (!read_pod(bytes, payload_bytes, sparse_end, rh) || rh.magic != HOLO_RESIDUAL_MAGIC || rh.version != 2 || rh.struct_size != sizeof(HoloResidualPayloadHeader) || rh.decoded_bytes != approx.size() || sparse_end + rh.residual_payload_bytes > payload_bytes) {
        out.error = HoloError::DecodeFailed;
        return out;
    }

    const auto* rp = bytes + sparse_end;
    if (rh.residual_payload_crc32 != 0 && crc32_bytes(rp, rh.residual_payload_bytes) != rh.residual_payload_crc32) {
        out.error = HoloError::IntegrityCheckFailed;
        return out;
    }

    std::vector<std::uint8_t> residual;
    if (!decompress_residual_payload(rp, rh.residual_payload_bytes, rh.residual_uncompressed_bytes, static_cast<HoloResidualCompression>(rh.compression), residual)) {
        out.error = HoloError::DecodeFailed;
        return out;
    }

    out.decoded = apply_xor_residual(approx.data(), residual.data(), static_cast<std::uint32_t>(residual.size()));
    if (rh.decoded_crc32 != 0 && crc32_bytes(out.decoded.data(), static_cast<std::uint32_t>(out.decoded.size())) != rh.decoded_crc32) {
        out.error = HoloError::IntegrityCheckFailed;
        return out;
    }

    out.error = HoloError::Ok;
    return out;
}
