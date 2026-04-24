# VVram-QH-Suite

Generated: 2026-04-24 14:43:25 Australia/Sydney

This repository is a monorepo-style container for the three internal Holo Suite project repos:

1. `holoram-core` — quantum-holographic memory core library.
2. `holovram-runtime` — virtual video RAM runtime using QH system-RAM warm backing.
3. `hololinux-integration` — Linux kernel/user integration, queue device, exact/lossless zswap path.

## Architecture rules

- `holoram-core` must not depend on graphics APIs, Vulkan, Direct3D, Linux zswap, or kernel code.
- `holovram-runtime` may depend on `holoram-core`.
- `hololinux-integration` may depend only on stable format/contracts from `holoram-core` or a kernel-safe exact codec subset.
- No CUDA runtime inside the Linux kernel.
- Approximate QH sparse modes must never be used for zswap unless residual correction makes recovery exact.
- vVRAM is tile-based, not arbitrary fake VRAM.
- Real VRAM is the hot tier; QH system RAM is the warm backing tier.
- WriteMap/generation metadata is mandatory for every slot/tile/page mapping.
- Every approximate mode must report BER, PSNR, SSIM, cosine similarity, or task-specific accuracy.

## Status

This is a structured v0.0.1 skeleton and design archive. Many source files are intentionally marked `TODO(v0.1)` so the next implementation steps can fill modules in a controlled order.

## Quick build smoke test

```bash
cd holoram-core
cmake -S . -B build -DHOLO_ENABLE_CUDA=OFF
cmake --build build -j
ctest --test-dir build
```

## Push instructions

See `GITHUB_PUSH_INSTRUCTIONS.md`.
