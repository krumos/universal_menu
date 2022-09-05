#include <malloc.h>
#include <stdio.h>
#include "../headers/menu.h"
#include "../headers/picker.h"

#define MENU_BACK_TEXT "Back"
#define MENU_EXIT_TEXT "Exit"
#define ACTIVE_COLOR "\033[1;31m"

struct menu
{
    struct Items *items;
};

void execute_menu_item(MENU menu, struct Node *node);

MENU create_menu()
{
    MENU menu = malloc(sizeof(struct menu));

    menu->items = malloc(sizeof(struct Items));
    init_list(menu->items);

    return menu;
}

void add_command(MENU menu, void(*func)(void), const char *description)
{
    struct Item item =
            {
                    .description = description,
                    .type = COMMAND_TYPE_ITEM,
                    .entity.func = func,
                    .is_active = DEFAULT_STATE
            };

    push_list(menu->items, &item);
}

void add_data_command(MENU menu, void(*func)(void *), const char *description, void *data)
{
    struct Item item =
            {
                    .description = description,
                    .type = COMMAND_W_DATA_TYPE_ITEM,
                    .entity.func_w_data = func,
                    .is_active = DEFAULT_STATE,
                    .data = data
            };

    push_list(menu->items, &item);
}

void add_exit(MENU menu, const char *description)
{
    struct Item item =
            {
                    .description = description,
                    .type = EXIT_TYPE_ITEM,
                    .is_active = DEFAULT_STATE
            };

    push_list(menu->items, &item);
}

void add_sub_menu(MENU menu, MENU sub_menu, const char *description)
{
    struct Item item =
            {
                    .description = description,
                    .type = MENU_TYPE_ITEM,
                    .entity.menu = sub_menu,
                    .is_active = DEFAULT_STATE
            };

    push_list(menu->items, &item);
    sub_menu->items->head->element->is_active = ACTIVE_STATE;
    add_exit(sub_menu, MENU_BACK_TEXT);
}

void print_menu(struct menu *menu)
{
    struct Node *node = menu->items->head;

    set_cursor_to_start();

    while (node != NULL)
    {
        struct Item *item = node->element;

        if (item->is_active == ACTIVE_STATE)
        {
            printf(ACTIVE_COLOR);
            printf("%s\n", item->description);
            printf("\033[0m");
        }
        else
        {
            printf("%s\n", item->description);
        }

        node = node->next;
    }
}

void print_back_menu()
{
    printf(ACTIVE_COLOR);
    printf("\n%s", MENU_BACK_TEXT);
    printf("\033[0m");
}

void select_next_item(MENU menu, struct Node **node)
{
    (*node)->element->is_active = DEFAULT_STATE;

    if ((*node)->next != NULL)
    {
        (*node)->next->element->is_active = ACTIVE_STATE;
        *node = (*node)->next;
    }
    else
    {
        *node = menu->items->head;
        (*node)->element->is_active = ACTIVE_STATE;
    }
}


void select_prev_item(MENU menu, struct Node **node)
{
    (*node)->element->is_active = DEFAULT_STATE;

    if ((*node)->prev != NULL)
    {
        (*node)->prev->element->is_active = ACTIVE_STATE;
        *node = (*node)->prev;
    }
    else
    {
        *node = menu->items->tail;
        (*node)->element->is_active = ACTIVE_STATE;
    }
}

void invoke_void_func(struct Node* node)
{
    node->element->entity.func();
}

void invoke_data_func(struct Node* node)
{
    node->element->entity.func_w_data(node->element->data);
}

void execute_command(MENU menu, struct Node *node, void(*invoke_func)(struct Node*))
{
    clear_console();
    invoke_func(node);
    print_back_menu();
    while (read_key() != ENTER_KEY);
    clear_console();
    print_menu(menu);

}

void menu_worker(struct menu *menu)
{
    clear_console();

    struct Node *node = menu->items->head;
    while (node->element->is_active != ACTIVE_STATE)
        node = node->next;

    print_menu(menu);
    while (1)
    {
        print_menu(menu);

        KEY_T c = read_key();

        switch (c)
        {
            case DOWN_KEY:
                select_next_item(menu, &node);
                break;
            case UP_KEY:
                select_prev_item(menu, &node);
                break;
            case ENTER_KEY:
                switch (node->element->type)
                {
                    case COMMAND_TYPE_ITEM:
                        execute_command(menu, node, invoke_void_func);
                        break;
                    case COMMAND_W_DATA_TYPE_ITEM:
                        execute_command(menu, node, invoke_data_func);
                        break;
                    case MENU_TYPE_ITEM:
                        execute_menu_item(menu, node);
                        break;
                    case EXIT_TYPE_ITEM:
                        clear_console();
                        return;
                }
        }
    }
}

void execute_menu_item(MENU menu, struct Node *node)
{
    clear_console();
    menu_worker(node->element->entity.menu);
    print_menu(menu);
}

void execute_menu(MENU menu)
{
    add_exit(menu, MENU_EXIT_TEXT);
    struct Node *node = menu->items->head;
    node->element->is_active = ACTIVE_STATE;
    menu_worker(menu);
}

void free_menu(MENU menu)
{
    struct Node *node = menu->items->head;

    while (node != NULL)
    {
        struct Node *next = node->next;

        if (node->element->type == MENU_TYPE_ITEM)
        {
            free_menu(node->element->entity.menu);
        }

        if (node->element->type == COMMAND_W_DATA_TYPE_ITEM)
        {
            free(node->element->data);
        }

        free_node(node);
        node = next;
    }
    free(menu->items);
    free(menu);
}
