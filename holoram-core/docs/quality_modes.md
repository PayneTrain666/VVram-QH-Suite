# Quality Modes

## Exact modes

- `ExactRaw`
- `ExactQhResidual`

These must recover bytes exactly.

## Approximate modes

- `ApproxVisual`
- `ApproxVector`
- `ApproxModel`

These must report quality metrics and must not be used for zswap unless residual protected.
