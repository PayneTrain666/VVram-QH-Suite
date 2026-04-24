# holovram-runtime Architecture

`holovram-runtime` manages virtual resource residency.

It does not make system RAM identical to VRAM. Instead, it uses:

- real VRAM as the hot resident tier
- QH system RAM as the warm backing tier
- disk/archive as the cold tier

Visible tiles should be materialized into real GPU resources before rendering or compute dispatch.
