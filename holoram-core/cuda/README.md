# CUDA Kernels

CUDA support is optional and controlled by `HOLO_ENABLE_CUDA`.

This directory will contain GPU implementations of:

- codebook upload
- sparse top-K selection
- sparse substrate write
- MMSE16 demix
- SIC refinement
- spectral shrink
- substrate kernels
- batch scheduler

The CPU path must remain valid without CUDA.
