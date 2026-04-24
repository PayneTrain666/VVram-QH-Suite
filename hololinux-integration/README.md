# hololinux-integration

Linux kernel/user integration for Holo Suite.

Owns:

- `/dev/holoram_kq`
- mmap submit/completion queues
- daemon
- kernel-safe slot budget policy
- stats bridge
- exact/lossless zswap backend
- fallback path
- DKMS/build scripts

## Hard rules

- No CUDA runtime inside the kernel.
- No unbounded waits in reclaim or page fault paths.
- zswap must use exact or residual-protected recovery.
- GPU offload is opportunistic only.
- Timeout always falls back.
- Handles must be generation-checked.
- Decoded pages must pass CRC before success.
