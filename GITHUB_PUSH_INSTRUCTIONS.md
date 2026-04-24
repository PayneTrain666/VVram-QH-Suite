# GitHub Push Instructions

Generated: 2026-04-24 14:43:25 Australia/Sydney

This repository has now been initialized through the GitHub connector and contains the generated Holo Suite monorepo structure.

## Repo

`PayneTrain666/VVram-QH-Suite`

## Manual clone

```bash
git clone https://github.com/PayneTrain666/VVram-QH-Suite.git
cd VVram-QH-Suite
```

## Smoke test: holoram-core

```bash
cd holoram-core
cmake -S . -B build -DHOLO_ENABLE_CUDA=OFF
cmake --build build -j
ctest --test-dir build
```

## Optional local artifact merge

If you also downloaded the local ZIP artifact from ChatGPT, you can compare it against the repo:

```bash
unzip VVram-QH-Suite.zip
rsync -av --dry-run VVram-QH-Suite/ ./
```

## Branching recommendation

Use feature branches for implementation phases:

```bash
git checkout -b feature/holoram-core-v0.1
# implement codebooks/codecs/demix/substrate

git checkout main
git checkout -b feature/holovram-runtime-v0.1
# implement simulator/runtime

git checkout main
git checkout -b feature/hololinux-integration-v0.1
# implement queue/zswap path
```
