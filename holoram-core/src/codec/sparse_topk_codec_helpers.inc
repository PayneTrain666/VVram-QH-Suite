namespace {

constexpr float kPi = 3.14159265358979323846f;

template <class T>
void append_pod(std::vector<std::uint8_t>& out, const T& value) {
    const auto* p = reinterpret_cast<const std::uint8_t*>(&value);
    out.insert(out.end(), p, p + sizeof(T));
}

template <class T>
bool read_pod(const std::uint8_t* p, std::uint32_t n, std::uint32_t& off, T& out) {
    if (off + sizeof(T) > n) return false;
    std::memcpy(&out, p + off, sizeof(T));
    off += static_cast<std::uint32_t>(sizeof(T));
    return true;
}

std::uint32_t codec_q(const HoloCodecConfig& cfg, const HoloCodebook& cb) {
    const auto q = cb.config().q;
    if (cfg.q != 0 && cfg.q != q) throw std::invalid_argument("codec q/codebook q mismatch");
    return q;
}

std::uint16_t qbits(HoloQuantMode m) { return m == HoloQuantMode::Int8Complex ? 8 : 16; }
float b2s(std::uint8_t v) { return (static_cast<float>(v) - 127.5f) / 127.5f; }

std::uint8_t s2b(float x) {
    const int y = static_cast<int>(std::lround((x * 127.5f) + 127.5f));
    return static_cast<std::uint8_t>(std::max(0, std::min(255, y)));
}

std::vector<Complex32> dft(const std::vector<float>& x) {
    const std::uint32_t q = static_cast<std::uint32_t>(x.size());
    const float norm = 1.0f / std::sqrt(static_cast<float>(q));
    std::vector<Complex32> y(q);
    for (std::uint32_t m = 0; m < q; ++m) {
        Complex32 acc{0.0f, 0.0f};
        for (std::uint32_t n = 0; n < q; ++n) {
            const float ph = -2.0f * kPi * static_cast<float>(m) * static_cast<float>(n) / static_cast<float>(q);
            acc += x[n] * Complex32{std::cos(ph), std::sin(ph)};
        }
        y[m] = acc * norm;
    }
    return y;
}

std::vector<float> idft_real(const std::vector<Complex32>& x) {
    const std::uint32_t q = static_cast<std::uint32_t>(x.size());
    const float norm = 1.0f / std::sqrt(static_cast<float>(q));
    std::vector<float> y(q);
    for (std::uint32_t n = 0; n < q; ++n) {
        Complex32 acc{0.0f, 0.0f};
        for (std::uint32_t m = 0; m < q; ++m) {
            const float ph = 2.0f * kPi * static_cast<float>(m) * static_cast<float>(n) / static_cast<float>(q);
            acc += x[m] * Complex32{std::cos(ph), std::sin(ph)};
        }
        y[n] = (acc * norm).real();
    }
    return y;
}

std::vector<float> make_block(const std::uint8_t* src, std::uint32_t total, std::uint32_t block, std::uint32_t q) {
    std::vector<float> x(q, 0.0f);
    const std::uint32_t base = block * q;
    for (std::uint32_t i = 0; i < q; ++i) if (base + i < total) x[i] = b2s(src[base + i]);
    return x;
}

void pack_topk(HoloSparseBlock& b, const std::vector<Complex32>& coeff, std::uint32_t q, std::uint16_t k, std::uint16_t qb) {
    b = HoloSparseBlock{};
    b.q = static_cast<std::uint16_t>(q);
    b.k = k;
    b.qbits = qb;
    std::vector<std::pair<float, std::uint16_t>> rank;
    rank.reserve(q);
    for (std::uint32_t i = 0; i < q; ++i) rank.emplace_back(std::norm(coeff[i]), static_cast<std::uint16_t>(i));
    std::partial_sort(rank.begin(), rank.begin() + k, rank.end(), [](auto& a, auto& z) { return a.first > z.first; });
    float max_abs = 0.0f;
    for (std::uint16_t i = 0; i < k; ++i) {
        const auto c = coeff[rank[i].second];
        max_abs = std::max(max_abs, std::max(std::abs(c.real()), std::abs(c.imag())));
    }
    b.scale = qb == 8 ? choose_i8_scale(max_abs) : choose_i16_scale(max_abs);
    for (std::uint16_t i = 0; i < k; ++i) {
        const auto idx = rank[i].second;
        const auto c = coeff[idx];
        b.idx[i] = idx;
        if (qb == 8) {
            b.re[i] = static_cast<std::int16_t>(clamp_i8(static_cast<int>(std::lround(c.real() / b.scale))));
            b.im[i] = static_cast<std::int16_t>(clamp_i8(static_cast<int>(std::lround(c.imag() / b.scale))));
        } else {
            b.re[i] = clamp_i16(static_cast<int>(std::lround(c.real() / b.scale)));
            b.im[i] = clamp_i16(static_cast<int>(std::lround(c.imag() / b.scale)));
        }
    }
}

bool sparse_header_ok(const HoloSparsePayloadHeader& h) {
    return h.magic == HOLO_SPARSE_PAYLOAD_MAGIC && h.version == 2 && h.struct_size == sizeof(HoloSparsePayloadHeader) && h.q != 0 && h.k != 0 && h.k <= HOLO_SPARSE_K_MAX && h.block_count != 0;
}

std::vector<std::uint8_t> sparse_reconstruct(const std::uint8_t* payload, std::uint32_t bytes, const HoloCodebook& cb, HoloError& err, std::uint32_t* end_offset) {
    err = HoloError::Ok;
    std::uint32_t off = 0;
    HoloSparsePayloadHeader h{};
    if (!read_pod(payload, bytes, off, h) || !sparse_header_ok(h)) { err = HoloError::DecodeFailed; return {}; }
    if (h.q != cb.config().q || h.channel >= cb.config().channels) { err = HoloError::InvalidArgument; return {}; }
    const std::uint32_t sparse_bytes = h.block_count * static_cast<std::uint32_t>(sizeof(HoloSparseBlock));
    if (off + sparse_bytes > bytes) { err = HoloError::DecodeFailed; return {}; }
    if (h.sparse_crc32 != 0 && crc32_bytes(payload + off, sparse_bytes) != h.sparse_crc32) { err = HoloError::IntegrityCheckFailed; return {}; }
    std::vector<std::uint8_t> approx(h.decoded_bytes, 0);
    const auto* code = cb.channel(h.channel);
    for (std::uint32_t bi = 0; bi < h.block_count; ++bi) {
        HoloSparseBlock b{};
        if (!read_pod(payload, bytes, off, b) || b.magic != HOLO_SPARSE_BLOCK_MAGIC || b.version != 1 || b.struct_size != sizeof(HoloSparseBlock) || b.q != h.q || b.k != h.k) { err = HoloError::DecodeFailed; return {}; }
        std::vector<Complex32> coeff(h.q, Complex32{0.0f, 0.0f});
        for (std::uint16_t i = 0; i < b.k; ++i) {
            if (b.idx[i] >= h.q) { err = HoloError::DecodeFailed; return {}; }
            const Complex32 enc{static_cast<float>(b.re[i]) * b.scale, static_cast<float>(b.im[i]) * b.scale};
            coeff[b.idx[i]] = enc * std::conj(code[b.idx[i]]);
        }
        const auto sig = idft_real(coeff);
        const std::uint32_t base = bi * h.q;
        for (std::uint32_t i = 0; i < h.q && base + i < h.decoded_bytes; ++i) approx[base + i] = s2b(sig[i]);
    }
    if (end_offset) *end_offset = off;
    return approx;
}

} // namespace
