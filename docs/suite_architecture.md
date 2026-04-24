# Holo Suite Architecture

Holo Suite is split into three repositories to prevent responsibility bleed.

```text
holoram-core
    ↑
    |
holovram-runtime       hololinux-integration
```

`holoram-core` owns the quantum-holographic storage science and reusable codec/substrate layer.

`holovram-runtime` owns virtual resource/tile residency and graphics/compute runtime behavior.

`hololinux-integration` owns kernel/user queueing, zswap integration, and Linux safety constraints.

## Memory hierarchy

```text
Real VRAM hot tier
  ↓
QH system-RAM warm tier
  ↓
Raw system RAM / SSD cold tier
```

Approximate storage is allowed for visual/vector/model-cache workloads. Exact page recovery is mandatory for zswap and any OS-level memory path.
