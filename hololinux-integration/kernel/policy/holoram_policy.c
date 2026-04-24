#include "../../include/kernel/holoram_policy.h"

u32 holoram_cost_per_block(u16 k)
{
    if (k <= 16) return 1;
    if (k <= 24) return 2;
    return 3;
}
