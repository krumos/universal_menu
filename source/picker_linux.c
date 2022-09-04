#ifdef __linux__
#include "../headers/keys.h"
#include "../headers/picker.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

void clear_console()
{
    system("clear");
}

void set_cursor_to_start()
{
    printf("\033[;H");
}

static struct termios old, current;

void initTermios()
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    current.c_lflag &= ~ECHO; /* set no echo mode */
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}


KEY_T read_key()
{
    initTermios();
    int c = fgetc(stdin);
    if (c == 27)
    {
        c = fgetc(stdin);
        if (c == 91)
        {
            c = fgetc(stdin);
            resetTermios();
            if (c == 65)
                return UP_KEY;
            if (c == 66)
                return DOWN_KEY;
        }
    }
    else if (c == 10)
    {
        resetTermios();
        return ENTER_KEY;
    }
    resetTermios();
    return DEFAULT_KEY;
}

#endif

