# Stats Bridge

The daemon periodically pushes slot hot/saturated state to the kernel.

The kernel uses this only to correct budget policy. The kernel must not block on live GPU stats.
