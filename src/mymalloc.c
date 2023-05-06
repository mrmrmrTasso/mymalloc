#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mymalloc.h"
#include "mylist.h"

void my_men_init() {
    pthread_mutexattr_init(&attr); // 初始化互斥锁属性
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); // 设置互斥锁属性为可重入
    pthread_mutex_init(&lock, &attr);
    pthread_mutex_lock(&lock);
    // 一次申请4M内存，也可以使用懒分配，调用my_malloc的时候再去分配
    my_mem = (char*)malloc(MEM_SIZE);
    pthread_mutex_unlock(&lock);
    if(NULL == my_mem) {
        printf("malloc failed");
        exit(1);
    }
    // 在分配内存的时候再初始化内存，这样效率会更高
    // memset(my_mem, 0, MEM_SIZE);
    mem_list = NULL;
    free_list = NULL;
    // 等内存吃紧的时候再去初始化
    my_mem_to = NULL;
    num = 0;
    cur = 0;
}

void my_compact() {
    pthread_mutex_lock(&lock);
    printf("my_compact...\n");
    // TODO
    // 使用复制算法，直接将内存拷贝到to区
    if (my_mem_to == NULL) {
        my_mem_to = (char*)malloc(MEM_SIZE);
        // printf("my_mem_to address: %p\n", my_mem_to);
    }
    int cur1 = 0;
    // 清除free链表
    list_clear(&free_list);
    struct Node* p = mem_list;
    while (p != NULL) {
        memcpy(my_mem_to + cur1, p->ptr, p->size);
        p->ptr = my_mem_to + cur1;
        cur1 += p->size;
        p = p->next;
    }
    char* tmp = my_mem;
    my_mem = my_mem_to;
    my_mem_to = my_mem;
    cur = cur1;
    pthread_mutex_unlock(&lock);
}

// 从free的区域中去找合适的内存
void* my_malloc1(unsigned int size) {
    printf("my_malloc1...\n");
    // 使用BF算法（实现简单，但不适用于大内存）
    if (free_list == NULL) {
        return NULL;
    }
    struct Node* node = free_list;
    free_list = free_list->next;
    node->next = NULL;
    
    if (node->size < size) {
        return NULL;
    } else if (node->size == size) {
        void* ret = node->ptr;
        list_insert(&mem_list, node);
        return ret;
    } else {
        struct Node* node1 = (struct Node*)malloc(sizeof(struct Node));
        memcpy(node1, node, sizeof(struct Node));
        node1->size = size;
        list_insert(&mem_list, node1);
        void* ret = node1->ptr;
        node->size = node->size - size;
        node->ptr = node->ptr + size;
        list_insert(&free_list, node);
        return ret;
    }
}

void* my_malloc(unsigned int size) {
    pthread_mutex_lock(&lock);
    // TODO: 检查的操作
    if (size == 0) {
        printf("参数值必须大于0！\n");
        exit(1);
    }
    if (cur + size > MEM_SIZE) {
        void* ret;
        if ((ret = my_malloc1(size)) == NULL) {
            // 触发compact
            my_compact();
            // 再次判断
            if (cur + size > MEM_SIZE) {
                printf("堆内存空间不足！\n");
                exit(1);
            }
        } else {
            memset(ret, 0, size);
            pthread_mutex_unlock(&lock);
            return ret;
        }
          
    }
    struct Node* node = malloc(sizeof(struct Node));
    if(NULL == node) {
        printf("malloc failed");
        exit(1);
    }
    // 初始化节点大小
    node->size = size;
    // 初始化节点的ptr
    num++;
    node->ptr = &my_mem[cur];
    cur = cur + size;
    // 将节点插入到mem_list
    list_insert(&mem_list, node);
    memset(node->ptr, 0, size);
    pthread_mutex_unlock(&lock);
    return node->ptr;
}


void my_free(void* ptr) {
    pthread_mutex_lock(&lock);
    struct Node* node = list_delete(&mem_list, ptr);
    // 将节点插入到mem_list
    list_insert(&free_list, node);
    pthread_mutex_unlock(&lock);
}

void my_mem_destroy() {
    free(my_mem);
    list_clear(&mem_list);
    list_clear(&free_list);
    pthread_mutexattr_destroy(&attr); // 销毁互斥锁属性
    pthread_mutex_destroy(&lock);
}


// 打印内存快照
void my_mem_dump() {
    printf("=================内存快照信息==================\n");
    print_list(mem_list);
}

// 打印内存快照
void my_detailed_mem_dump() {
    printf("=================内存快照详细信息==================\n");
    printf("快照起始地址：%p\n", my_mem);
    printf("mem_list信息：\n");
    print_list(mem_list);
    printf("free_list信息：\n");
    print_list(free_list);
}

