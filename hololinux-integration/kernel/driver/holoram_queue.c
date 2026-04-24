#include <linux/uaccess.h>
#include "../../include/uapi/holoram_kq_uapi.h"

long holokq_get_info_ioctl(unsigned long arg)
{
    struct holokq_info info = {
        .version = HOLOKQ_UAPI_VERSION,
        .struct_size = sizeof(struct holokq_info),
        .submit_capacity = 0,
        .complete_capacity = 0,
        .job_size = sizeof(struct holokq_job),
        .flags = 0,
    };
    if (copy_to_user((void __user *)arg, &info, sizeof(info))) return -EFAULT;
    return 0;
}
