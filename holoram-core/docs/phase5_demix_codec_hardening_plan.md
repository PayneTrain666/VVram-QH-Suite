# HoloRAM Core Phase 5 Plan — Demix/Codec Integration Hardening

## Purpose

Phase 4 added the public demix API, CRC hardening, residual compression hooks, corruption detection, and benchmark scaffolding.

Phase 5 should convert those pieces into a more robust internal codec/demix integration layer so sparse QH storage can move from reference-only behavior toward a dependable core library.

## Current state

Implemented already:

- public demix API: matched, MMSE, MMSE+SIC
- CRC32 helpers
- sparse payload CRC checks
- residual payload CRC checks
- residual compression hooks: None, RLE, AutoRLE
- sparse corruption detection test
- demix benchmark scaffold
- GitHub Actions CI workflow for `holoram-core`

Known risks:

- sparse codec still uses CPU O(Q^2) DFT/IDFT
- MMSE path currently uses a direct small dense solve and no cached Gram inverse
- sparse codec does not yet expose reusable block-level encode/decode internals
- payload format needs explicit version migration policy
- residual compression is simple RLE only
- benchmark output is basic text, not CSV/JSON
- GPU/CUDA path is still stubbed

## Phase 5 implementation target

### 1. Demix integration

Add reusable block-level demix helpers:

- `build_channel_observation(...)`
- `demix_sparse_observation(...)`
- `decode_sparse_block_with_demix(...)`

The codec should be able to select demix mode from `HoloCodecConfig.demix_mode`.

### 2. Precomputed Gram cache

Add a small CPU-side Gram cache keyed by:

- codebook strategy
- Q
- channels
- seed
- salt
- lambda

This should avoid rebuilding the MMSE system for every block.

### 3. Payload format hardening

Add clear helpers:

- `validate_sparse_payload_header(...)`
- `validate_residual_payload_header(...)`
- `sparse_payload_header_to_string(...)`

Keep versioned structs with `version` and `struct_size` checks.

### 4. CRC coverage tests

Add tests for:

- sparse block corruption
- residual header corruption
- residual body corruption
- decoded CRC mismatch
- truncated payload
- invalid magic
- wrong struct size

### 5. Benchmark reporting

Upgrade benchmarks to emit CSV rows with columns:

```text
phase,mode,q,c,k,quant,codec,payload_bytes,ratio,ber,psnr,encode_ms,decode_ms,residual_norm
```

### 6. Exact mode guardrail

Add explicit guard tests proving:

- `ExactRaw` roundtrips exactly
- `ExactQhResidual` roundtrips exactly
- approximate modes do not claim exactness
- zswap-compatible paths must only use exact modes

## Non-goals for Phase 5

Do not implement CUDA yet.
Do not implement Vulkan/D3D/vVRAM runtime here.
Do not implement Linux zswap code here.
Do not replace the whole codec with a highly optimized FFT path yet.

## Acceptance criteria

Phase 5 is complete when:

- GitHub Actions passes for `holoram-core`
- all local tests pass under Ninja on a normal Linux machine
- corruption tests fail closed with `HoloError::IntegrityCheckFailed` or `HoloError::DecodeFailed`
- demix benchmark can compare Matched vs MMSE vs MMSE+SIC across Q/C/K matrix
- sparse codec still compiles without CUDA

## Recommended next command

```text
Implement Repo 1 Phase 5: demix/codec integration hardening.

Use the existing holoram-core Phase 4 code as the source of truth.

Implement:
- reusable sparse payload/header validation helpers
- block-level demix integration helpers
- Gram cache for MMSE CPU demix
- corruption tests for sparse block, residual header, residual payload, decoded CRC mismatch, truncation, bad magic, bad struct size
- CSV benchmark output for sparse density and demix benchmarks
- exact mode guardrail tests

Keep C++17 compatibility.
Do not add CUDA yet.
Do not touch holovram-runtime or hololinux-integration except documentation if absolutely required.
Budget tokens based on complexity requirements if that is allowed.
If you are going to run out of space ask me to continue.
```
