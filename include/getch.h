// getch.h
#ifndef GETCH_H
#define GETCH_H

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    static int getch(void) {
        struct termios oldt, newt;
        int ch;

        if (tcgetattr(STDIN_FILENO, &oldt) != 0)
            return -1;

        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);

        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
            return -1;

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    }
#endif

#endif