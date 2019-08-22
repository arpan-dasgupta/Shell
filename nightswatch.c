#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
// #include "conio.h"

int toInt(char *val)
{
    int ans = 0;
    for (int i = 0; i < strlen(val); i++)
    {
        ans = ans * 10 + (val[i] - '0');
    }
    return ans;
}

void enable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int _kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

void nightint(char *time)
{
    int interval = toInt(time);
    struct timeval t1, t2;
    double elapsedTime;
    gettimeofday(&t1, NULL);
    while (1)
    {
        enable_raw_mode();
        if (!(_kbhit()))
        {
            disable_raw_mode();
            gettimeofday(&t2, NULL);
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
            if (elapsedTime >= interval * 1000)
            {
                printf("hi\n");
                gettimeofday(&t1, NULL);
            }
        }
        else
        {
            disable_raw_mode();
            char i = getchar();
            if (i == 'q')
                break;
        }
    }
}

void nightdirt(char *time)
{
}