# Kernel Safety Rules

- No CUDA runtime in kernel.
- No unbounded waits in reclaim/page fault paths.
- zswap must recover pages exactly.
- GPU offload is opportunistic.
- Timeout must fall back.
- CRC/generation validation is mandatory.
