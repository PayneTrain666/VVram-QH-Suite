#include "../../include/kernel/holoram_exact_codec.h"
#include <linux/string.h>
#include <linux/errno.h>

int holoram_exact_encode_page_cpu(const void *page, void *out, u32 out_cap, u32 *out_len)
{
    if (!page || !out || !out_len) return -EINVAL;
    if (out_cap < 4096) return -ENOSPC;
    memcpy(out, page, 4096);
    *out_len = 4096;
    return 0;
}

int holoram_exact_decode_page_cpu(const void *payload, u32 payload_len, void *page, u32 page_bytes)
{
    if (!payload || !page) return -EINVAL;
    if (payload_len > page_bytes) return -EINVAL;
    memcpy(page, payload, payload_len);
    return 0;
}
