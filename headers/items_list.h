#ifndef UNIVERSAL_MENU_ITEMS_LIST_H
#define UNIVERSAL_MENU_ITEMS_LIST_H

#include "menu.h"
#include "stdio.h"
#include "item.h"

typedef struct Node Node;

struct Node
{
    struct Item *element;
    Node *prev;
    Node *next;
};

struct Items
{
    Node *head;
    Node *tail;
    size_t count;
};

struct Node *init_list(struct Items *items);

struct Node *push_list(struct Items *items, struct Item *element);

void free_node(Node *node);

#endif //UNIVERSAL_MENU_ITEMS_LIST_H
