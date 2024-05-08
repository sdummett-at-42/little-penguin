// SPDX-License-Identifier: GPL-2.0-only
/*
 * fortytwo-debufs.c - A debugfs module.
 */

/*	What are jiffies ?
 *	- jiffies: Jiffies are a fundamental time unit used in the Linux kernel,
 *	represented by an unsigned long variable. The value of jiffies is
 *	typically incremented by the kernel timer interrupt at a fixed rate,
 *	often expressed in Hertz (Hz). The exact duration of a jiffy can vary
 *	across different systems depending on the kernel configuration and hardware.
 *
 *	- jiffies_64: Jiffies_64, on the other hand, are a version of jiffies
 *	that use a 64-bit integer data type to represent time.
 *	This allows for a much larger range of representable values compared
 *	to regular jiffies. It's used in situations where the 32-bit jiffies
 *	variable might overflow due to the passage of time, especially
 *	in systems with high uptime or when working with long-duration timers.
 */

#include <linux/init.h> /* Needed for the macros */
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/debugfs.h> /* Needed for operations on the debufs */
#include <linux/jiffies.h> /* Needed for jiffies */

#define LOGIN "sdummett" /* When cdev is read this is the output & is compared to that when written */
#define LOGIN_LEN 9 /* Login length includes the final NULL */
#define MODULE_NAME "fortytwo-debugfs" /* The module name */

static struct dentry *fortytwo_dir;
static char foo_data[PAGE_SIZE];
static DEFINE_MUTEX(mtx);

static ssize_t foo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = -EINVAL;

	mutex_lock(&mtx);

	retval = simple_read_from_buffer(buf, count, f_pos, foo_data, strlen(foo_data));

	mutex_unlock(&mtx);

	return retval;
}

static ssize_t foo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = -EINVAL;

	if (count > PAGE_SIZE)
		return retval;

	mutex_lock(&mtx);

	retval = simple_write_to_buffer(foo_data, count, f_pos, buf, count);
	foo_data[count - 1] = 0x0;

	mutex_unlock(&mtx);

	return retval;
}

/* The 'foo' file operation structure */
const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

/* Return the LOGIN when 'id' is read */
static ssize_t id_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, LOGIN, LOGIN_LEN);
}

/* If the written value match LOGIN return the count else return EINVAL */
static ssize_t id_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char user_msg[LOGIN_LEN];
	ssize_t retval = -EINVAL;

	if (count != LOGIN_LEN)
		return retval;

	retval = simple_write_to_buffer(user_msg, count, f_pos, buf, count);
	if (retval < 0)
		return retval;

	retval = strncmp(LOGIN, user_msg, count - 1) ? -EINVAL : count;
	return retval;
}

/* The 'id' file operation structure */
const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

static int fail(void)
{
	debugfs_remove_recursive(fortytwo_dir);
	pr_alert("Could not create devices");
	return -ENODEV;
}

/* Init the debugfs module */
static int __init fortytwo_init(void)
{
	pr_info("%s: Installing the directory tree.\n", MODULE_NAME);

	pr_info("%s: Creating debugfs subdirectory named 'fortytwo'.\n", MODULE_NAME);
	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	if (!fortytwo_dir)
		return fail();

	pr_info("%s: Creating the '%s' file.\n", MODULE_NAME, "id");
	if (!debugfs_create_file("id", 0666, fortytwo_dir, NULL, &id_fops))
		return fail();

	pr_info("%s: Creating the '%s' file.\n", MODULE_NAME, "jiffies");
	debugfs_create_u64("jiffies", 0444, fortytwo_dir, (u64 *)&jiffies_64);

	pr_info("%s: Creating the '%s' file.\n", MODULE_NAME, "foo");
	if (!debugfs_create_file("foo", 0644, fortytwo_dir, NULL, &foo_fops))
		return fail();

	pr_info("%s: Successfully installed the directory tree.\n", MODULE_NAME);
	return 0;
}

/* Cleanup the debugfs module */
static void __exit fortytwo_exit(void)
{
	pr_info("%s: Removing debugfs subdirectory 'fortytwo'.\n", MODULE_NAME);
	debugfs_remove_recursive(fortytwo_dir);
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me>");
MODULE_DESCRIPTION("A debugfs module.");
