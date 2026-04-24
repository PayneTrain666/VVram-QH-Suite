#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include "../../include/uapi/holoram_kq_uapi.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Holo Suite");
MODULE_DESCRIPTION("HoloRAM kernel/user queue device");
MODULE_VERSION("0.0.1");

long holokq_get_info_ioctl(unsigned long arg);
long holokq_stats_push_ioctl(unsigned long arg);

static long holokq_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    (void)file;
    switch (cmd) {
    case HOLOKQ_IOC_GET_INFO:
        return holokq_get_info_ioctl(arg);
    case HOLOKQ_IOC_STATS_PUSH:
        return holokq_stats_push_ioctl(arg);
    default:
        return -ENOTTY;
    }
}

static const struct file_operations holokq_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = holokq_ioctl,
    .llseek = no_llseek,
};

static struct miscdevice holokq_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = HOLOKQ_DEVICE_NAME,
    .fops = &holokq_fops,
    .mode = 0660,
};

static int __init holokq_init(void)
{
    int ret = misc_register(&holokq_misc);
    if (ret) pr_err("holoram_kq: misc_register failed: %d\n", ret);
    else pr_info("holoram_kq: loaded\n");
    return ret;
}

static void __exit holokq_exit(void)
{
    misc_deregister(&holokq_misc);
    pr_info("holoram_kq: unloaded\n");
}

module_init(holokq_init);
module_exit(holokq_exit);
