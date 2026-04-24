# HoloRAM Core Phase 4 Content Record

Generated from local package: `VVram-QH-Suite.zip`

## Phase 4 scope

Repo 1 Phase 4 implements demix + residual hardening for `holoram-core`.

Implemented items:

- matched-filter public demix API
- MMSE demix API
- SIC refinement
- CRC32 in `HoloBlockHeader`
- CRC32 in sparse payload headers
- CRC32 in residual payload headers
- residual compression hooks: `None`, `Rle`, `AutoRle`
- corruption-detection tests
- demix benchmark covering matched vs MMSE vs MMSE+SIC
- benchmark dimensions: `C=8/16/32`, `Q=256/512/1024`, `K=8/16/24/32/48/64`

## New files

```text
holoram-core/include/holoram/qh_demix.h
holoram-core/src/metrics/crc32.cpp
holoram-core/tests/test_crc32.cpp
holoram-core/tests/test_sparse_corruption_detection.cpp
```

## Major updated files

```text
holoram-core/CMakeLists.txt
holoram-core/VERSION
holoram-core/include/holoram/core.h
holoram-core/include/holoram/formats/holo_block.h
holoram-core/include/holoram/formats/holo_sparse_block.h
holoram-core/include/holoram/qh_codec.h
holoram-core/include/holoram/qh_metrics.h
holoram-core/include/holoram/types.h
holoram-core/include/holoram/version.h
holoram-core/src/codec/residual_codec.cpp
holoram-core/src/codec/sparse_topk_codec.cpp
holoram-core/src/demix/matched_filter.cpp
holoram-core/src/demix/mmse.cpp
holoram-core/src/demix/sic.cpp
holoram-core/tests/CMakeLists.txt
holoram-core/tests/test_mmse_demix.cpp
holoram-core/tests/test_sic_demix.cpp
holoram-core/tests/test_sparse_exact_residual.cpp
holoram-core/tests/test_sparse_topk_payload.cpp
holoram-core/benches/bench_demix_mmse.cpp
holoram-core/benches/bench_sparse_density.cpp
```

## Validation record

Local affected-file validation reported:

```text
CRC32 test passed
MMSE demix test passed
SIC demix test passed
sparse exact residual test passed
sparse payload header test passed
sparse corruption detection test passed
```

A full CMake/Ninja build was reported as successful for the affected repo package in the local sandbox, with 11/11 tests passing after the earlier CMake/codebook fixes.

## Important source design details

### Public demix API

The Phase 4 API introduces `HoloDemixConfig`, `HoloDemixResult`, and functions:

```cpp
HoloDemixResult matched_filter_demix(const std::vector<Complex32>& y, const HoloCodebook& codebook, const HoloDemixConfig& cfg);
HoloDemixResult mmse_demix(const std::vector<Complex32>& y, const HoloCodebook& codebook, const HoloDemixConfig& cfg);
HoloDemixResult sic_demix(const std::vector<Complex32>& y, const HoloCodebook& codebook, const HoloDemixConfig& cfg);
HoloDemixResult demix_channels(const std::vector<Complex32>& y, const HoloCodebook& codebook, const HoloDemixConfig& cfg);
std::vector<Complex32> synthesize_from_channel_values(const HoloCodebook& codebook, const std::vector<Complex32>& channel_values);
double residual_l2_norm(const std::vector<Complex32>& a, const std::vector<Complex32>& b);
```

### CRC hardening

Phase 4 adds CRC helper functions:

```cpp
std::uint32_t crc32_bytes(const void* data, std::uint32_t bytes);
std::uint32_t crc32_extend(std::uint32_t previous_crc, const void* data, std::uint32_t bytes);
```

### Residual safety rule

Approximate QH storage remains approximate unless residual correction is stored and validated.

```text
approximate_sparse_decode XOR residual = exact_original_bytes
```

CRC validation must fail closed with `HoloError::IntegrityCheckFailed` when corrupted payloads are detected.
