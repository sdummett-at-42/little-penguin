/*
 * fortytwo.c - A misc character device driver.
 */

#include <linux/init.h> /* Needed for the macros */
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/miscdevice.h> /* Needed for struct miscdevice, etc */

#define LOGIN "sdummett" /* When cdev is read this is the output & is compared to that when written */
#define LOGIN_LEN 9 /* Login length includes the final NULL */
#define CHARDEV_NAME "fortytwo" /* The cdev name stored at -> /dev/fortytwo */

/* To prove that the code is indeed working:
 * - Insert the module into the kernel
 * - Read the misc cdev node / write to it.
 */

/* Return the LOGIN when /dev/fortytwo is read */
static ssize_t fortytwo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, LOGIN, \
		LOGIN_LEN);
}

/* If the written value match LOGIN return the count else return EINVAL */
static ssize_t fortytwo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
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

/* The file operation structure */
const struct file_operations fortytwo_fops = {
	.owner = THIS_MODULE,
	.read = fortytwo_read,
	.write = fortytwo_write,
};

/* The misc device structure */
static struct miscdevice fortytwo = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = CHARDEV_NAME,
	.fops = &fortytwo_fops,
};

/* Init the misc character device*/
static int __init fortytwo_init(void)
{
    pr_info("%s: loaded with minor number %i.\n", CHARDEV_NAME, fortytwo.minor);
    return misc_register(&fortytwo);
}

/* Cleanup the misc character device */
static void __exit fortytwo_exit(void)
{
	pr_info("%s: unloaded.\n", CHARDEV_NAME);
	misc_deregister(&fortytwo);
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me>");
MODULE_DESCRIPTION("A misc character device driver named fortytwo");
