#ifndef ADD_DEL_VECTOR_H
#define ADD_DEL_VECTOR_H
int register_vector(struct vector *new_vec);
int deregister_vector(unsigned int vect_num);
void * get_vector_address(unsigned int vect_num);
struct vector_list *get_vector_list_head(void);
#endif
