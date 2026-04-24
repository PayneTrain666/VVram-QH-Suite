#pragma once

#include <linux/types.h>
#include <linux/ioctl.h>

#define HOLOKQ_UAPI_VERSION 1
#define HOLOKQ_DEVICE_NAME "holoram_kq"
#define HOLOKQ_IOCTL_BASE 'H'
#define HOLOKQ_IOC_GET_INFO   _IOR(HOLOKQ_IOCTL_BASE, 0x01, struct holokq_info)
#define HOLOKQ_IOC_STATS_PUSH _IOW(HOLOKQ_IOCTL_BASE, 0x30, struct holokq_stats_push)
#define HOLOKQ_JOB_ECHO        1
#define HOLOKQ_JOB_ENCODE_PAGE 2
#define HOLOKQ_JOB_DECODE_PAGE 3
#define HOLOKQ_STATUS_PENDING  0
#define HOLOKQ_STATUS_OK       1
#define HOLOKQ_STATUS_FAIL     2
#define HOLOKQ_STATUS_TIMEOUT  3

struct holokq_info {
    __u32 version;
    __u32 struct_size;
    __u32 submit_capacity;
    __u32 complete_capacity;
    __u32 job_size;
    __u32 flags;
};

struct holokq_job {
    __u32 version;
    __u32 struct_size;
    __u64 job_id;
    __u32 client_id;
    __u32 generation;
    __u16 type;
    __u16 status;
    __u16 profile_id;
    __u16 flags;
    __u64 in_offset;
    __u32 in_bytes;
    __u32 reserved0;
    __u64 out_offset;
    __u32 out_cap_bytes;
    __u32 out_bytes;
    __u32 err_code;
    __u32 crc32;
};

struct holokq_slot_stat {
    __u32 slot_id;
    __u16 flags;
    __u16 reserved0;
    __u32 budget_hint;
};

struct holokq_stats_push {
    __u32 version;
    __u32 struct_size;
    __u32 n;
    __u32 epoch_seen;
    /* flexible array follows: struct holokq_slot_stat stats[]; */
};
