#ifndef _MYLIST_H_
#define _MYLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

void list_insert(struct Node** list, struct Node* node);

void list_clear(struct Node** list);

struct Node* list_delete(struct Node** list, void* ptr);

struct Node* list_find(struct Node* list, void* ptr);

void print_list(struct Node* list);

#endif