#ifndef UNIVERSAL_MENU_ITEM_H
#define UNIVERSAL_MENU_ITEM_H

#include "menu.h"

enum TYPE
{
    COMMAND_TYPE_ITEM,
    COMMAND_W_DATA_TYPE_ITEM,
    MENU_TYPE_ITEM,
    EXIT_TYPE_ITEM,
};

enum IS_ACTIVE
{
    ACTIVE_STATE,
    DEFAULT_STATE,
};

struct Item
{
    const char *description;
    enum TYPE type;
    enum IS_ACTIVE is_active;

    union
    {
        void (*func)(void);

        void (*func_w_data)(void *);

        struct menu *menu;
    } entity;

    void *data;
};
#endif //UNIVERSAL_MENU_ITEM_H
