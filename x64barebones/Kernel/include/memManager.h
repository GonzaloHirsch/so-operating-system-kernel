#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdint.h>

enum STATE {FREE = 1, NOT_FREE = 0};

typedef struct t_node * Node;

void * m_alloc(size_t size);
int m_free(void * ptr);
void init_list(void * start_dir, size_t total_size);

#endif
