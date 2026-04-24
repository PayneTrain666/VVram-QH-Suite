#pragma once

#include <linux/types.h>

struct holo_zswap_handle {
    u32 slot_id;
    u16 channel_base;
    u16 profile_id;
    u16 k_sparse;
    u16 mode;
    u32 generation;
    u32 crc32;
    u32 decoded_bytes;
};
