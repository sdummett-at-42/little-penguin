// SPDX-License-Identifier: GPL-2.0-only
/*
* mymounts.c - List mount points on the system
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>

#define PROCFS_NAME "mymounts"

static struct proc_dir_entry *procfile;

static int mymounts_show(struct seq_file *m, void *v)
{
	struct rb_node *node;

	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	for (node = rb_first(&ns->mounts); node; node = rb_next(node)) {
		struct mount *mnt = rb_entry(node, struct mount, mnt_node);

		struct path mnt_path = { .dentry = mnt->mnt.mnt_root, .mnt = &mnt->mnt };
		struct super_block *sb = mnt_path.dentry->d_sb;

		if (!strcmp(mnt->mnt_devname, "rootfs"))
				continue ;

		if (sb->s_op->show_devname)
				sb->s_op->show_devname(m, mnt_path.dentry);
		else
				seq_puts(m, mnt->mnt_devname ? mnt->mnt_devname : "none");
		seq_putc(m, ' ');
		seq_path(m, &mnt_path, " \t\n\\");
		seq_putc(m, '\n');
	}
	return 0;
}

static int mymounts_open(struct inode *inode, struct file *file)
{
	return single_open(file, mymounts_show, NULL);
}

static const struct proc_ops mymounts_proc_ops = {
	.proc_open = mymounts_open,
	.proc_read = seq_read,
};

static int __init mymounts_init(void)
{
	procfile = proc_create(PROCFS_NAME, 0644, NULL, &mymounts_proc_ops);
	if (NULL == procfile) {
		proc_remove(procfile);
		pr_alert("%s: Error: Could not initialize /proc/%s\n", PROCFS_NAME, PROCFS_NAME);
		return -ENOMEM;
	}

	pr_info("%s: /proc/%s created.\n", PROCFS_NAME, PROCFS_NAME);
	return 0;
}

static void __exit mymounts_exit(void)
{
	proc_remove(procfile);
	pr_info("%s: /proc/%s removed.\n", PROCFS_NAME, PROCFS_NAME);
}

module_init(mymounts_init);
module_exit(mymounts_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stone <sdummett.dev@proton.me>");
MODULE_DESCRIPTION("List mount points on the system in the /proc/mymounts file.");
