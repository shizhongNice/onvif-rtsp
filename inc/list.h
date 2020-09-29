#ifndef _LIST_H_
#define _LIST_H_

struct list_node;
struct list;

typedef struct list_node {
    unsigned char* ptr;
	struct list_node* next;
	struct list_node* prev;
	struct list* plist;
} list_node_t;

typedef struct list {
	list_node_t *head;
	list_node_t *tail;
} list_t;

int x_list_init(list_t* plist);
list_t* x_list_alloc(void);
void x_list_free(list_t* plist);

int x_list_isempty(list_t* plist);
int x_list_insert_head(list_t* plist, list_node_t* pnode);
int x_list_insert_head(list_t* plist, list_node_t* pnode);

list_node_t* x_list_get_tail(list_t* plist);
list_node_t* x_list_get_head(list_t* plist);

int  x_list_del(list_node_t* pnode);


#endif
