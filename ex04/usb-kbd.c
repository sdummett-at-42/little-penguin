/*
 * usb-kbd.c - Usb keyboard module.
 */

#include <linux/init.h> /* Needed for the macros */ 
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

static int __init usb_kbd_init(void)
{
	pr_info("usb-kbd: Usb keyboard module loaded!\n");
	/* A non 0 return means the init failed; module can't be loaded. */
	return 0;
}

static void __exit usb_kbd_exit(void)
{
	pr_info("usb-kbd: Usb keyboard module unloaded.\n");
}

module_init(usb_kbd_init);
module_exit(usb_kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me>");
MODULE_DESCRIPTION("An usb keyboard module");
