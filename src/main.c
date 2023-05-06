#include "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>

// size: 16byte
typedef struct A {
    double d_value;
    int i_value;
} A;

// 测试各种数据类型的使用
void test01() {
    printf("=================test01()==================\n");
    my_men_init();
    struct A* ap = (struct A*)my_malloc(sizeof(struct A));
    ap->d_value = 1.3;
    ap->i_value = 12;
    printf("%f %d\n", ap->d_value, ap->i_value);
    int* arr = (int*)my_malloc(sizeof(int)*100);
    int i;
    for (i = 0; i < 10; ++i) {
        arr[i] = i;
        printf("arr[%d]的值为：%d\n", i, arr[i]);
    }
    my_detailed_mem_dump();
    my_free(arr);
    my_detailed_mem_dump();
    my_mem_destroy();
}

// 测试free区域是否可以复用
void test02() {
    printf("=================test02()==================\n");
    my_men_init();
    int* arr = (int*)my_malloc(sizeof(int)*1024*1024);
    my_detailed_mem_dump();
    my_free(arr);
    my_detailed_mem_dump();
    int* arr2 = (int*)my_malloc(sizeof(int)*1024);
    my_detailed_mem_dump();
    my_mem_destroy();
}

void test03() {
    printf("=================test03()==================\n");
    my_men_init();
    int** arr;
    arr = (int**)my_malloc(16*sizeof(int*));
    for (int i = 0; i < (16+8); ++i) {
        arr[i] = (int*)my_malloc(64*1024*sizeof(int));
        if (i % 2) {
            my_free(arr[i]);
        }
    }
    my_detailed_mem_dump();
    my_mem_destroy();
}

void test04() {
    // 1 1 释放1 2 2 释放2 4
    printf("=================test04()==================\n");
    my_men_init();
    int block_size = 128*1024*sizeof(int);
    int* p1 = (int*)my_malloc(block_size);
    int* p2 = (int*)my_malloc(block_size);
    my_free(p2);
    int* p3 = (int*)my_malloc(2*block_size);
    int* p4 = (int*)my_malloc(2*block_size);
    my_free(p4);
    my_detailed_mem_dump();
    int* p5 = (int*)my_malloc(4*block_size);

    my_detailed_mem_dump();
    my_mem_destroy();
}

int main() {
    // test01();
    // test02();
    // test03();
    test04();
    return 0;
}