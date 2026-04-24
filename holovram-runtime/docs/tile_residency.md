# Tile Residency

Tiles move through states:

```text
ResidentDisk
ResidentQHSystem
DecodePending
UploadPending
ResidentVRAM
Evicted
Failed
```

The runtime must track generation, quality, and residency for every tile.
