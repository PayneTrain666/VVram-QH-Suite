#include <linux/errno.h>
#include "../../include/kernel/holoram_policy.h"

int holoram_pick_slot_budget(struct holoram_policy *pol, struct holoram_slot_state *slots, u32 slots_total, u32 preferred_slot, u32 cost_per_page, u32 *chosen_slot)
{
    u32 s0;
    int b0;
    u32 best;
    int best_b;
    if (!pol || !slots || !chosen_slot || slots_total == 0) return -EINVAL;
    s0 = preferred_slot % slots_total;
    b0 = atomic_read(&slots[s0].budget);
    if ((u32)b0 >= pol->slot_budget_hot && (u32)b0 >= cost_per_page) { *chosen_slot = s0; return 0; }
    best = s0;
    best_b = b0;
    for (u32 t = 0; t < pol->max_redirect_tries; ++t) {
        u32 s = holoram_mix32(preferred_slot ^ (pol->epoch + t * 0x9e3779b9U)) % slots_total;
        int b = atomic_read(&slots[s].budget);
        if (b > best_b) { best_b = b; best = s; }
        if ((u32)b >= pol->slot_budget_hot && (u32)b >= cost_per_page) { *chosen_slot = s; return 0; }
    }
    if ((u32)best_b >= pol->slot_budget_deny && (u32)best_b >= cost_per_page) { *chosen_slot = best; return 0; }
    return -ENOSPC;
}
