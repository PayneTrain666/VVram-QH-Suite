# Roadmap

## Stage 1 — holoram-core MVP

- codebook generation
- dense exact codec
- sparse approximate codec
- demix
- substrate
- WriteMap
- tests and benchmarks

## Stage 2 — holovram-runtime simulator

- tile manager
- simulated VRAM cache
- QH backing tier
- virtual texture simulator

## Stage 3 — hololinux queue

- /dev/holoram_kq
- mmap rings
- daemon echo jobs
- timeout/fallback tests

## Stage 4 — graphics runtime

- Vulkan sparse backend
- D3D12 tiled backend
- CUDA compute backend

## Stage 5 — exact zswap

- exact raw mode
- exact QH+residual mode
- CRC/generation checking
- fallback compressor

## Stage 6 — advanced optimization

- resource-aware DCT/wavelet/PCA transforms
- precomputed MMSE inverse
- Tensor Core denoiser
- GPUDirect Storage cold path
