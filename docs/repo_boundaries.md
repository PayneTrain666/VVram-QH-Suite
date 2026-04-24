# Repository Boundaries

## holoram-core

Allowed:

- QH math
- codec implementation
- substrate implementation
- WriteMap/WAL
- CPU/GPU codec kernels
- metrics

Forbidden:

- Vulkan
- Direct3D
- Linux kernel module code
- zswap-specific logic
- game-engine-specific logic

## holovram-runtime

Allowed:

- tile manager
- residency system
- VRAM cache
- QH warm-tier adapter
- graphics/compute backends
- prefetch
- quality policy

Forbidden:

- implementing QH math directly instead of calling holoram-core
- Linux kernel code
- zswap logic

## hololinux-integration

Allowed:

- kernel module
- queue ABI
- daemon
- zswap exact backend
- stats bridge
- DKMS packaging

Forbidden:

- CUDA runtime in kernel
- approximate-only page recovery
- graphics API resource management
