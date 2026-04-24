#pragma once

#include <linux/types.h>
#include <linux/atomic.h>

#define HOLO_BLOCKS_PER_PAGE 8

struct holoram_policy {
    u16 channels;
    u16 depth_slices;
    u16 k_write;
    u16 k_read;
    u32 epoch;
    u32 max_redirect_tries;
    u32 slot_budget_init;
    u32 slot_budget_hot;
    u32 slot_budget_deny;
    u32 max_inflight_jobs;
};

struct holoram_slot_state {
    atomic_t budget;
    atomic_t inflight;
    u32 generation;
    u32 flags;
};

static inline u32 holoram_mix32(u32 x)
{
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

static inline u16 holoram_channel_for(u32 slot_id, u16 block_idx, u32 epoch, u16 depth, u16 channels)
{
    u32 x = slot_id ^ (epoch + 0x9e3779b9U);
    x ^= (u32)depth * 0x85ebca6bU;
    x ^= (u32)block_idx * 0xc2b2ae35U;
    x = holoram_mix32(x);
    return channels ? (u16)(x % channels) : 0;
}
