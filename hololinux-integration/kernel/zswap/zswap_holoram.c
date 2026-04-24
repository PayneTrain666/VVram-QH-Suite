/*
 * TODO(v0.1): Implement zswap compressor/wrapper.
 * Rules:
 * - exact raw mode first
 * - exact QH+residual mode second
 * - approximate sparse-only mode forbidden for zswap
 * - CRC must verify decoded page
 * - timeout must fall back
 */
