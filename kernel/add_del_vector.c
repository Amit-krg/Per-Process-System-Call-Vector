#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/add_del_vector.h>
#include <linux/vector_list.h>

int register_vector(struct vector *new_vec)
{
	struct vector_list *temp;

	mutex_lock(&list_mutex);
	temp = list_head;
	if (!new_vec) {
		printk("Vector cannot be NULL\n");
		return -EINVAL;
	}
	if (!list_head) {
		list_head = kmalloc(sizeof(*temp), GFP_KERNEL);
		if (!list_head) {
			printk("Could not allocate memory\n");
			mutex_unlock(&list_mutex);
			return -ENOMEM;
		}
		list_head->next = NULL;
		list_head->vec = new_vec;
	} else {
		while (temp) {
			if (temp->vec->vector_num == new_vec->vector_num) {
				printk("Vector number %d already exists\n",
				       new_vec->vector_num);
				mutex_unlock(&list_mutex);
				return -EEXIST;
			}
			if (!temp->next) {
				temp->next = kmalloc(sizeof(*temp), GFP_KERNEL);
				if (!temp->next) {
					printk("Could not allocate memory\n");
					return -ENOMEM;
				}
				temp->next->vec = new_vec;
				temp->next->next = NULL;
				break;
			}
			temp = temp->next;
		}
	}
	mutex_unlock(&list_mutex);
	return 0;
}
EXPORT_SYMBOL(register_vector);

int deregister_vector(unsigned int vect_num)
{
	struct vector_list *tmp;
	struct vector_list *tmp2;

	mutex_lock(&list_mutex);
	tmp = list_head;
	if (list_head->vec->vector_num == vect_num) {
		list_head = list_head->next;
		kfree(tmp);
		mutex_unlock(&list_mutex);
		return 0;
	}
	tmp2 = tmp->next;
	while (tmp2) {
		if (tmp2->vec->vector_num == vect_num) {
			tmp->next = tmp2->next;
			kfree(tmp2);
			mutex_unlock(&list_mutex);
			return 0;
		}
		tmp2 = tmp2->next;
		tmp = tmp->next;
	}
	printk("Could not find vector number %d\n", vect_num);
	mutex_unlock(&list_mutex);
	return -EINVAL;
}
EXPORT_SYMBOL(deregister_vector);

void *get_vector_address(unsigned int vect_num)
{
	struct vector_list *tmp;

	mutex_lock(&list_mutex);
	tmp = list_head;
	while (tmp) {
		if (tmp->vec->vector_num == vect_num) {
			mutex_unlock(&list_mutex);
			return tmp;
		}
		tmp = tmp->next;
	}
	printk("Could not find vector number %d\n", vect_num);
	mutex_unlock(&list_mutex);
	return NULL;
}
EXPORT_SYMBOL(get_vector_address);

struct vector_list *get_vector_list_head(void)
{
	struct vector_list *tmp;

	mutex_lock(&list_mutex);
	tmp = list_head;
	mutex_unlock(&list_mutex);

	return tmp;
}
EXPORT_SYMBOL(get_vector_list_head);
