# holoram-core

`holoram-core` is the quantum-holographic memory core library for Holo Suite.

It owns:

- QH codebooks
- CAZAC/Zadoff-Chu and MUB-style code generation
- dense and sparse codecs
- MMSE/SIC demixing
- spectral denoising
- persistent substrate slot storage
- slot/channel policies
- WriteMap/WAL
- quality metrics
- tests and benchmarks

## Non-goals

This repo must not include:

- Vulkan
- Direct3D
- zswap
- Linux kernel modules
- graphics resource management

## Build

```bash
cmake -S . -B build -DHOLO_ENABLE_CUDA=OFF
cmake --build build -j
ctest --test-dir build
```
