#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 从unistd中导入sleep函数
#include<unistd.h>
// 导入需要使用的线程库
#include <pthread.h>

// 内存大小 这里是默认值，可以不用写死
#define MEM_SIZE 1024*1024*4

// 真正的内存快照，可以理解成from区
char* my_mem;

// to区
char* my_mem_to;


// 已分配的区域数量
int num;

// 当前分配的位置，最大应该为MEM_SIZE
int cur;

// 互斥锁属性
pthread_mutexattr_t attr;

// 内存是全局共享的，因此需要互斥访问
// 后续也可以考虑做成线程独享的分离式内存池，定义一个线程专有的锁
pthread_mutex_t lock;

typedef struct Node {
    // 内存块大小
    int size;
    // next节点
    struct Node* next;
    // 具体指向的区域
    void* ptr;
} Node;

// 两个有序列表
struct Node* mem_list;
struct Node* free_list;

void my_men_init();

void* my_malloc(unsigned int size);

void my_free(void* ptr);

void my_mem_destroy();

void my_mem_dump();

void my_detailed_mem_dump();

void my_compact();

#endif