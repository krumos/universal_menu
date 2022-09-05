#ifndef UNIVERSAL_MENU_MENU_H
#define UNIVERSAL_MENU_MENU_H

#include "stdio.h"
#include "items_list.h"

struct menu;

typedef struct menu* MENU;

MENU create_menu();

void add_command(MENU menu, void(*func)(void), const char *description);

void add_command_with_data(MENU menu, void(*func)(void *), const char *description, void *data);

void execute_menu(MENU menu);

void add_sub_menu(MENU menu, MENU sub_menu, const char *description);

void free_menu(MENU menu);

#endif //UNIVERSAL_MENU_MENU_H
