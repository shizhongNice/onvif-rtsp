#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

int x_list_init(list_t* plist)
{
	plist->head = NULL;
	plist->tail = NULL;
    return 0;
}

list_t* x_list_alloc(void)
{
    list_t* plist= malloc(sizeof(list_t));
	plist->head = NULL;
	plist->tail = NULL;
    return plist;
}

int  x_list_isempty(list_t* plist)
{
	if (NULL == plist) {
		printf("error:(%s,%d\n)",__func__,__LINE__);
		return -1;
	}

	return (NULL == plist->head);
}

int x_list_insert_tail(list_t* plist, list_node_t* pnode)
{
	if (NULL == plist|| NULL == pnode){
		printf("error:(%s,%d),param is invalid.\n",__func__,__LINE__);
		return -1;
	}

	if(NULL == plist->head) {
		plist->head = pnode;
		plist->tail = pnode;
        pnode->prev = NULL;
        pnode->next = NULL;
    } else {
		plist->tail->next = pnode;
        pnode->prev = plist->tail;
        pnode->next = NULL;
        plist->tail = pnode;
    }
    pnode->plist = plist;

	return 0;
}

int x_list_insert_head(list_t* plist, list_node_t* pnode)
{
	if (NULL == plist|| NULL == pnode){
		printf("error:(%s,%d),param is invalid.\n",__func__,__LINE__);
		return -1;
	}

	if(NULL == plist->head) {
		plist->head = pnode;
		plist->tail = pnode;
        pnode->prev = NULL;
        pnode->next = NULL;
    } else {
		plist->head->prev = pnode;
        pnode->prev = NULL;
        pnode->next = plist->head;
        plist->head = pnode;
    }
    pnode->plist = plist;

	return 0;
}

list_node_t* x_list_get_head(list_t* plist)
{
	if (NULL == plist){
		printf("error:(%s,%d),param is invalid.\n",__func__,__LINE__);
		return NULL;
	}
    return plist->head;
}

list_node_t* x_list_get_tail(list_t* plist)
{
	if (NULL == plist){
		printf("error:(%s,%d),param is invalid.\n",__func__,__LINE__);
		return NULL;
	}
    return plist->tail;
}


int x_list_del(list_node_t* pnode)
{

	if (NULL == pnode){
		printf("error:*(%s,%d),pnode is empty.\n",__func__,__LINE__);
		return -1;
	}
    if ((NULL == pnode->prev)&&(NULL == pnode->next)) {
        assert(pnode == pnode->plist->head);
        assert(pnode == pnode->plist->tail);
        pnode->plist->head=NULL;
        pnode->plist->tail=NULL;
        return 0;
    }

    if (NULL == pnode->prev) {
        assert(pnode == pnode->plist->head);
        pnode->plist->head=pnode->next;
        pnode->next->prev=NULL;
    } else {
        pnode->prev->next = pnode->next;
    }

    if (NULL == pnode->next) {
        assert(pnode == pnode->plist->tail);
        pnode->plist->tail = pnode->prev;
        pnode->prev->next = NULL;
    } else {
        pnode->next->prev = pnode->prev;
    }

	return 0;
}

# if 0
int test1()
{
    int i = 0;
    int ret = 0;
    list_t list;
    ret = list_init(&list);
    assert(0 == ret);
    list_node_t node[20];
    for (i = 0; i < 20; i++) {
        node[i].ptr = (unsigned char* )(unsigned long)i;
        ret = list_insert_head(&list, &node[i]);
        assert(0 == ret);
    }
    list_node_t* pnode;
    for (i = 0; i < 20; i++) {
        pnode = list_get_tail(&list);
        assert(0 != pnode);
        assert((unsigned char* )(unsigned long)i == pnode->ptr);
        ret = list_del(pnode);
        assert(0 == ret);
    }
    assert(NULL == list_get_tail(&list));
    assert(NULL == list_get_head(&list));
    printf("%s, ok\n", __func__);
    return 0;
}

int test2()
{
    int i = 0;
    int ret = 0;
    list_t list;
    ret = list_init(&list);
    assert(0 == ret);
    list_node_t node[20];
    for (i = 0; i < 20; i++) {
        node[i].ptr = (unsigned char* )(unsigned long)i;
        ret = list_insert_tail(&list, &node[i]);
        assert(0 == ret);
    }
    list_node_t* pnode;
    for (i = 0; i < 20; i++) {
        pnode = list_get_head(&list);
        assert(0 != pnode);
        assert((unsigned char* )(unsigned long)i == pnode->ptr);
        ret = list_del(pnode);
        assert(0 == ret);
    }
    assert(NULL == list_get_tail(&list));
    assert(NULL == list_get_head(&list));
    printf("%s, ok\n", __func__);
    return 0;
}
int test3()
{
    int i = 0;
    int ret = 0;
    list_t list;
    ret = list_init(&list);
    assert(0 == ret);
    list_node_t node[20];
    for (i = 0; i < 20; i++) {
        node[i].ptr = (unsigned char* )(unsigned long)i;
        ret = list_insert_head(&list, &node[i]);
        assert(0 == ret);
    }
    list_node_t* pnode;
    for (i = 19; i >= 0; i--) {
        pnode = list_get_head(&list);
        assert(0 != pnode);
        assert((unsigned char* )(unsigned long)i == pnode->ptr);
        ret = list_del(pnode);
        assert(0 == ret);
    }
    assert(NULL == list_get_tail(&list));
    assert(NULL == list_get_head(&list));
    printf("%s, ok\n", __func__);
    return 0;
}
int test4()
{
    int i = 0;
    int ret = 0;
    list_t list;
    ret = list_init(&list);
    assert(0 == ret);
    list_node_t node[20];
    for (i = 0; i < 20; i++) {
        node[i].ptr = (unsigned char* )(unsigned long)i;
        ret = list_insert_tail(&list, &node[i]);
        assert(0 == ret);
    }
    list_node_t* pnode;
    for (i = 19; i >= 0; i--) {
        pnode = list_get_tail(&list);
        assert(0 != pnode);
        assert((unsigned char* )(unsigned long)i == pnode->ptr);
        ret = list_del(pnode);
        assert(0 == ret);
    }
    assert(NULL == list_get_tail(&list));
    assert(NULL == list_get_head(&list));
    printf("%s, ok\n", __func__);
    return 0;
}
int test5()
{
    int i = 0;
    int ret = 0;
    list_t list;
    ret = list_init(&list);
    assert(0 == ret);
    list_node_t node[20];
    for (i = 0; i < 20; i++) {
        node[i].ptr = (unsigned char* )(unsigned long)i;
        ret = list_insert_tail(&list, &node[i]);
        assert(0 == ret);
    }
    list_node_t* pnode;
    for (i = 19; i >= 0; i--) {
        pnode = list_get_tail(&list);
        assert(0 != pnode);
        assert((unsigned char* )(unsigned long)i == pnode->ptr);
        ret = list_del(pnode);
        assert(0 == ret);
    }
    assert(NULL == list_get_tail(&list));
    assert(NULL == list_get_head(&list));
    printf("%s, ok\n", __func__);
    return 0;
}
int main(int argc, unsigned char *argv[])
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
#endif
