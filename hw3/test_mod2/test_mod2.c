#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/compat.h>
#include <linux/mount.h>
#include <linux/fs.h>
#include <linux/add_del_vector.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

struct vector *vec;

unsigned long __fdget_pos(unsigned int fd)
{
	unsigned long v = __fdget(fd);
	struct file *file = (struct file *)(v & ~3);

	if (file && (file->f_mode & FMODE_ATOMIC_POS)) {
		if (file_count(file) > 1) {
			v |= FDPUT_POS_UNLOCK;
			mutex_lock(&file->f_pos_lock);
		}
	}
	return v;
}

static inline struct fd fdget_pos(int fd)
{
	return __to_fd(__fdget_pos(fd));
}

static inline void fdput_pos(struct fd f)
{
	if (f.flags & FDPUT_POS_UNLOCK)
		mutex_unlock(&f.file->f_pos_lock);
	fdput(f);
}

long my_creat(const char __user *pathname, umode_t mode)
{
	return sys_open(pathname, O_CREAT | O_WRONLY | O_TRUNC, mode);
}


long my_read(unsigned int fd, char __user *buf, size_t count)
{
	unsigned int i,j;
	char str[50],ext[5];
	struct fd f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	printk("in my_read\n");
	
	if (f.file->f_path.dentry->d_iname != NULL) {	
		i = 0, j = 0;
		strcpy(str, f.file->f_path.dentry->d_iname);
		 while(i < strlen(str) && str[i]!='.') i++;
		 while(i < strlen(str)) {
		 		ext[j]=str[i]; 
		 		j++ , i++;
		}
	}
	
        if (f.file && (ext == NULL || !strcmp(ext, "c") || !strcmp(ext, "pl")
		|| !strcmp(ext, "cpp"))) {
                loff_t pos = f.file->f_pos;
                ret = vfs_read(f.file, buf, count, &pos);
                if (ret >= 0)
                        f.file->f_pos = pos;
                fdput_pos(f);
        }
        return ret;
}

static int __init init_test_mod(void)
{
	int ret;

	vec = kmalloc(sizeof(*vec), GFP_KERNEL);
	if (vec == NULL)
		return -ENOMEM;

	memset(vec, 0, sizeof(*vec));

	vec->vector_num = 2;
	vec->sys_func[IND_READ].notallowed = 0;
	vec->sys_func[IND_READ].sys_call = &my_read;
	vec->sys_func[IND_CREAT].notallowed = 0;
	vec->sys_func[IND_CREAT].sys_call = &my_creat;
	vec->mod = THIS_MODULE;
	ret = register_vector(vec);
	if (ret != 0)
		return ret;
	return 0;
}
static void  __exit exit_test_mod(void)
{
	int ret = 0;

	ret = deregister_vector(2);
	if (ret != 0)
		return;
	if (!ret)
		kfree(vec);
}
module_init(init_test_mod);
module_exit(exit_test_mod);
MODULE_LICENSE("GPL");
