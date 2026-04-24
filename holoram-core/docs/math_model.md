# QH Math Model

The core storage model uses complex superposition:

```text
H_s[k] += alpha * U_c[k] * X[k]
```

Where:

- `H_s` is slot `s`
- `U_c` is the channel code
- `X` is the encoded spectrum
- `alpha` is the write scale

Reads demix by matched filtering, MMSE, or MMSE+SIC.
