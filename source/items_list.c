#include <string.h>
#include <malloc.h>
#include "../headers/items_list.h"

struct Node *init_list(struct Items *items)
{
    items->head = NULL;
    items->tail = NULL;
    items->count = 0;
    return items->head;
}

struct Node *push_list(struct Items *items, struct Item *element)
{
    void *p = malloc(sizeof(struct Item));

    if (p == NULL)
        return items->head;

    Node *node = malloc(sizeof(Node));

    memcpy(p, element, sizeof(struct Item));

    node->element = p;
    node->next = NULL;
    node->prev = items->tail;
    if (node->prev != NULL)
        node->prev->next = node;
    if (items->head == NULL)
        items->head = node;
    items->tail = node;
    items->count++;

    return items->head;
}


void free_node(Node *node)
{
    free(node->element);
    free(node);
}

