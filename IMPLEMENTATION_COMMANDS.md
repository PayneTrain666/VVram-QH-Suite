# Implementation Commands

Generated: 2026-04-24 Australia/Sydney

Use these commands in future ChatGPT/Codex sessions to generate and implement Holo Suite module-by-module.

## Command 1 — holoram-core skeleton

Create the full initial source-file pack for repo 1: `holoram-core`.

Goal: Build the clean quantum-holographic memory core library that other repos depend on.

Include codebooks, codecs, demix, substrate, WriteMap/WAL, metrics, tests, benchmarks, and examples.

Design requirements:
- Keep public API clean and stable.
- Separate exact/lossless modes from approximate QH modes.
- Include compile-friendly stubs, not just pseudocode.
- Use C++17 and optional CUDA extension later.
- Budget tokens based on complexity requirements if that is allowed.
- If you are going to run out of space ask me to continue.

## Command 2 — codebooks and coherence

Implement CAZAC/Zadoff-Chu and MUB/DFT-style codebooks, deterministic salting, coherence reports, tests, and benchmarks.

## Command 3 — dense and sparse codecs

Implement dense exact, sparse Top-K approximate, int8/int16 complex quantization, residual correction, and quality tests.

## Command 4 — demix

Implement matched filter, precomputed-inverse MMSE, SIC refinement, residual norms, channel energy, confidence metrics, and benchmarks.

## Command 5 — substrate + WriteMap

Implement persistent CPU substrate, energy accounting, admission policy, WriteMap records, WAL append/replay, generation checks, and recovery tests.

## Command 6 — CUDA substrate

Implement persistent GPU `H_fft[slot,Q]`, sparse Top-K kernels, sparse writes, MMSE16, SIC, spectral shrink, slot stats, and throughput benchmarks.

## Command 7 — holovram-runtime skeleton

Create the virtual video RAM runtime with tile/resource management, residency state, QH warm backing, prefetch, graphics backend interfaces, tests, and examples.

## Command 8 — vVRAM simulator

Implement resource/tile simulator, VRAM cache, QH backing, disk backing, eviction, frame/camera/mip predictors, and virtual texture simulation.

## Command 9 — CUDA runtime backend

Implement async decode/copy queues, CUDA residency, model-weight and embedding cache demos.

## Command 10 — Vulkan backend

Implement sparse image/buffer abstraction, tile upload, fences, fallback mip support, and virtual texture demo.

## Command 11 — D3D12 backend

Implement tiled resources, tile mapping, copy queue, fences, and D3D12 virtual texture demo.

## Command 12 — hololinux-integration skeleton

Create kernel queue device, UAPI, daemon, kernel-safe policy, exact codec skeleton, zswap skeleton, tools, and CI.

## Command 13 — queue device and daemon

Implement `/dev/holoram_kq`, mmap rings, versioned ABI, client IDs, generation fields, echo tests, daemon crash tests, and timeout fallback.

## Command 14 — kernel policy and stats bridge

Implement per-slot budget ledger, deterministic channel router, redirect/deny logic, stats push ioctl, and policy selftests.

## Command 15 — exact zswap backend

Implement exact raw mode, exact QH+residual mode, CRC verification, generation-checked handles, CPU fallback, timeout fallback, and zswap roundtrip tests.

## Command 16 — benchmark suite

Implement BER, PSNR, SSIM, cosine, timing, sparse density sweeps, MMSE/SIC quality sweeps, and CSV reporting.

## Command 17 — shared contracts

Define holoformat, holowire, holores, versioning, generation, exact/approximate rules, failure modes, and quality mode specs.

## Command 18 — end-to-end vVRAM demo

Simulate a 16384x16384 virtual texture with QH backing, moving camera, visible tile prediction, VRAM cache, fallback mips, and metrics.

## Command 19 — AI cache demo

Implement model-weight tile paging, embedding approximate storage, cosine metrics, and KV-cache approximate tier simulation.

## Command 20 — hard review

Review compile risks, ABI mismatches, performance bottlenecks, unsafe kernel assumptions, incorrect approximate/exact mode usage, missing tests, and patch priorities.
