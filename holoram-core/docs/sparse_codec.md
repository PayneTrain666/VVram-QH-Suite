# Sparse Codec

Sparse mode keeps only top-K spectral bins.

This mode is approximate unless paired with a residual stream.

Recommended early values:

- Q = 256
- C = 16
- K = 16, 24, 32
