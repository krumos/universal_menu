#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include "../headers/picker.h"
KEY_T read_key()
{
    char c = getch();
    switch (c)
    {
        case 0x50: return DOWN_KEY;
        case 0x48: return UP_KEY;
        case 13: return ENTER_KEY;
        default: return DEFAULT_KEY;
    }
}

void clear_console()
{
    system("cls");
}

void set_cursor_to_start()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(consoleHandle, pos);
}
#endif
