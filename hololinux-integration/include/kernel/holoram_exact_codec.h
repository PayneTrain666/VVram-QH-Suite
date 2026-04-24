#pragma once

#include <linux/types.h>

int holoram_exact_encode_page_cpu(const void *page, void *out, u32 out_cap, u32 *out_len);
int holoram_exact_decode_page_cpu(const void *payload, u32 payload_len, void *page, u32 page_bytes);
