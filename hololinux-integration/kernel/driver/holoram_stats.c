#include <linux/uaccess.h>
#include "../../include/uapi/holoram_kq_uapi.h"

long holokq_stats_push_ioctl(unsigned long arg)
{
    struct holokq_stats_push hdr;
    if (copy_from_user(&hdr, (void __user *)arg, sizeof(hdr))) return -EFAULT;
    if (hdr.version != HOLOKQ_UAPI_VERSION) return -EINVAL;
    /* TODO(v0.1): Copy flexible stats array and update slot budget ledger. */
    return 0;
}
