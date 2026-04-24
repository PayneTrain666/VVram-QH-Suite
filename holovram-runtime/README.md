# holovram-runtime

`holovram-runtime` is the virtual video RAM runtime for Holo Suite.

It manages:

- graphics/compute resources
- tile residency
- real VRAM hot cache
- QH system-RAM warm backing tier
- disk cold tier
- prefetch
- resource quality policy

It depends on `holoram-core`.

## Important rule

This is not fake VRAM. It is a tile/resource residency runtime.

Real VRAM remains the hot tier. QH system RAM is the warm backing tier.
