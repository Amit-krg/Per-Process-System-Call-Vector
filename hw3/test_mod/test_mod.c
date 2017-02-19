#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/add_del_vector.h>

struct vector *vec;

long my_mkdir(const char __user *pathname, umode_t mode)
{
	kuid_t uid = current_uid();

	printk("In new my_mkdir\n");
	if (uid.val == 0)
		return sys_mkdirat(AT_FDCWD, pathname, mode);
	else
		return -EPERM;
}

long my_creat(const char __user *filename, int flags, umode_t mode)
{
	kuid_t uid = current_uid();

	printk("In new my_rmdir\n");
	if (uid.val == 0)
		return sys_open(filename, flags, mode);
	else
		return -EPERM;
}


long my_symlink(const char __user *old, const char __user *new)
{
	kuid_t uid = current_uid();

	printk("In new my_symlink\n");
	if (uid.val == 0)
		return sys_symlinkat(old, AT_FDCWD, new);
	else
		return -EPERM;
}

long my_link(const char __user *old, const char __user *new)
{
	kuid_t uid = current_uid();

	printk("In new my_link\n");
	if (uid.val == 0)
		return sys_linkat(AT_FDCWD, old, AT_FDCWD, new, 0);
	else
		return -EPERM;
}

long my_rename(const char __user *old, const char __user *new)
{
	kuid_t uid = current_uid();

	printk("In new my_rename\n");
	if (uid.val == 0)
		return sys_renameat2(AT_FDCWD, old, AT_FDCWD, new, 0);
	else
		return -EPERM;
}



static int __init init_test_mod(void)
{
	int ret;

	vec = kmalloc(sizeof(*vec), GFP_KERNEL);
	if (vec == NULL)
		return -ENOMEM;

	memset(vec, 0, sizeof(*vec));

	vec->vector_num = 1;
	vec->sys_func[IND_MKDIR].notallowed = 0;
	vec->sys_func[IND_MKDIR].sys_call = &my_mkdir;
	vec->sys_func[IND_RMDIR].notallowed = 1;
	vec->sys_func[IND_UNLINK].notallowed = 1;
	vec->sys_func[IND_LINK].notallowed = 0;
	vec->sys_func[IND_LINK].sys_call = &my_link;
	vec->sys_func[IND_SYMLINK].notallowed = 0;
	vec->sys_func[IND_SYMLINK].sys_call = &my_symlink;
	vec->sys_func[IND_RENAME].notallowed = 0;
	vec->sys_func[IND_RENAME].sys_call = &my_rename;
	vec->mod = THIS_MODULE;
	ret = register_vector(vec);
	if (ret != 0)
		return ret;
	return 0;
}
static void  __exit exit_test_mod(void)
{
	int ret = 0;

	ret = deregister_vector(1);
	if (ret != 0)
		return;
	if (!ret)
		kfree(vec);
}
module_init(init_test_mod);
module_exit(exit_test_mod);
MODULE_LICENSE("GPL");
