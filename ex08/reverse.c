// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me");
MODULE_DESCRIPTION("When /dev/reverse is read it reverse what has been written.");

static DEFINE_MUTEX(mtx);
char str[PAGE_SIZE];
char *tmp;

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t t, i;
	ssize_t res;
	/***************
	 * Malloc like a boss
	 ***************/
	tmp = kmalloc(sizeof(char) * PAGE_SIZE * 2, GFP_KERNEL);
	for (t = strlen(str) - 1, i = 0; i < strlen(str); t--, i++)
		tmp[i] = str[t];
	tmp[i] = 0x0;

	mutex_lock(&mtx);

	res = simple_read_from_buffer(user, size, offs, tmp, i);
	kfree(tmp);

	mutex_unlock(&mtx);
	return res;
}

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;

	mutex_lock(&mtx);

	res = 0;
	res = simple_write_to_buffer(str, size, offs, user, size);
	str[size - 1] = 0x0;

	mutex_unlock(&mtx);
	return res;
}

const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static int __init myfd_init(void)
{
	pr_info("%s: loaded.\n", "reverse");
	return misc_register(&(*(&(myfd_device))));
}

static void __exit myfd_cleanup(void)
{
	pr_info("%s: unloaded.\n", "reverse");
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);
