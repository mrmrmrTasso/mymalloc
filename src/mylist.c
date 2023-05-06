#include "mylist.h"

// 不是头插法，按从大到小排序，有一定的时间复杂度
void list_insert(struct Node** list, struct Node* node) {
    if (*list == NULL) {
        *list = node;
        return;
    }

    if ((*list)->size <= node->size) {
        struct Node* p = (*list);
        (*list) = node;
        node->next = p;
        return;
    }
    struct Node* cur = *list;
    
    // 使cur指向要插入位置的前一个位置
    while (cur->next != NULL && cur->next->size > node->size) {
        cur = cur->next;
    }
    struct Node* p = cur->next;
    cur->next = node;
    node->next = p;
    
}

void list_clear(struct Node** list) {
    struct Node* p;
    while (*list != NULL) {
        p = *list;
        *list = (*list)->next;
        free(p);
    }
    
}

struct Node* list_delete(struct Node** list, void* ptr) {
    if (*list == NULL) {
        return NULL;
    }
    if ((*list)->ptr == ptr) {
        struct Node* p = *list;
        *list = (*list)->next;
        p->next = NULL;
        return p;
    }
    struct Node* cur = *list;
    while (cur->next != NULL) {
        if (cur->next->ptr == ptr) {
            struct Node* p = cur->next;
            cur->next = cur->next->next;
            p->next = NULL;
            return p;
        }
        
    }
    return NULL;
}

struct Node* list_find(struct Node* list, void* ptr) {
    struct Node* cur = list;
    while (cur != NULL) {
        if (cur->ptr == ptr) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void print_list(struct Node* list) {
    struct Node* cur = list;
    while (cur != NULL) {
        printf("memory size: %d\t\taddress: %p\t\toffset: %dbyte\n", cur->size, cur->ptr, (int)cur->ptr-(int)my_mem);
        cur = cur->next;
    }
}