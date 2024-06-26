/*
 * hello-world.c - Hello World module.
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

int init_module(void)
{
	pr_info("Hello world !\n");
	/* A non 0 return means init_module failed; module can't be loaded. */
	return 0;
}

void cleanup_module(void)
{
	pr_info("Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me>");
MODULE_DESCRIPTION("An hello world module");
