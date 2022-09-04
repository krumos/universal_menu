cmake_minimum_required(VERSION 3.16.3)
project(universal_menu)

set(CMAKE_C_STANDARD 11)

add_executable(universal_menu source/items_list.c headers/items_list.h source/menu.c headers/menu.h main.c source/picker_linux.c headers/picker.h headers/keys.h source/picker_windows.c  headers/item.h headers/macro_magic.h args.h)
