#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include "cd.h"

extern int alphasort();

#define clear() printf("\033[H\033[J")

int file_select(const struct dirent *entry)
{
    int y = ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) ? 0 : 1;
    return y;
}

void listfiles(char home[], char *dir, int l_flag, int a_flag)
{
    struct dirent **files;
    int count, i;
    // printf("%s\n", home);
    if (strlen(dir) == 0)
    {
        dir = (char *)malloc(2);
        dir[0] = '.';
        dir[1] = '\0';
    }
    if (dir[0] == '~')
    {
        int sz = strlen(home) + strlen(dir) - 1;
        char *temp = dir;
        dir = (char *)malloc(strlen(home) + strlen(dir));
        for (i = sz - 1; i >= strlen(home); i--)
        {
            dir[i] = temp[i - strlen(home) + 1];
        }
        for (i = 0; i < strlen(home); i++)
            dir[i] = home[i];
        dir[sz] = '\0';
    }
    char curdir[1024];
    getcwd(curdir, sizeof(curdir));
    cd(home, dir);
    char newdir[1024];
    getcwd(newdir, sizeof(newdir));
    cd(home, curdir);
    // printf("%s ++ \n", dir);
    count = scandir(dir, &files, file_select, alphasort);
    if (count < 0)
        perror("Error ");
    if (l_flag == 0)
    {
        if (a_flag == 1)
        {
            char *x = ".";
            printf("%s\n", x);
            char *y = "..";
            printf("%s\n", y);
        }
        for (i = 1; i < count + 1; ++i)
        {
            char *x = files[i - 1]->d_name;
            if (a_flag == 1 || x[0] != '.')
                printf("%s\n", x);
        }
    }
    else
    {
        struct stat statres;
        char x[1000];
        int p = strlen(newdir);
        lstat(newdir, &statres);
        // lstat(newdir, &statres);
        if (newdir[p - 1] != '/')
        {
            newdir[p] = '/';
            newdir[p + 1] = '\0';
        }
        for (int i = 0; i < strlen(newdir); i++)
        {
            x[i] = newdir[i];
        }
        long int szz = 0;
        for (i = 1; i < count + 1; ++i)
        {
            char *filname = files[i - 1]->d_name;
            for (int j = 0; j < strlen(filname); j++)
            {
                x[j + strlen(newdir)] = filname[j];
            }
            x[strlen(newdir) + strlen(filname)] = '\0';
            // printf("%s\n", x);
            if (a_flag == 1 || filname[0] != '.')
            {
                if (stat(x, &statres) >= 0)
                {
                    szz += statres.st_blocks / 2;
                }
            }
        }
        if (a_flag == 1)
        {
            char *filname = "..";
            if (strcmp(curdir, "/") != 0)
            {
                cd(home, "..");
                char backdir[1024];
                getcwd(backdir, sizeof(newdir));
                cd(home, curdir);

                if (stat(backdir, &statres) >= 0)
                {
                    szz += statres.st_blocks / 2;
                }
            }
        }
        if (a_flag == 1)
        {
            char *filname = ".";
            // printf("%s\n", x);
            if (stat(curdir, &statres) >= 0)
            {
                szz += statres.st_blocks / 2;
            }
        }
        printf("total %ld\n", szz);
        count = scandir(dir, &files, file_select, alphasort);
        if (a_flag == 1)
        {
            char *filname = ".";
            // printf("%s\n", x);
            if (stat(curdir, &statres) >= 0)
            {
                mode_t bits = statres.st_mode;
                char perm[11];
                if (S_ISREG(statres.st_mode))
                    perm[0] = '-';
                else
                    perm[0] = 'd';
                if ((bits & S_IRUSR) != 0)
                    perm[1] = 'r';
                else
                    perm[1] = '-';
                if ((bits & S_IWUSR) != 0)
                    perm[2] = 'w';
                else
                    perm[2] = '-';
                if ((bits & S_IXUSR) != 0)
                    perm[3] = 'x';
                else
                    perm[3] = '-';
                if ((bits & S_IRGRP) != 0)
                    perm[4] = 'r';
                else
                    perm[4] = '-';
                if ((bits & S_IWGRP) != 0)
                    perm[5] = 'w';
                else
                    perm[5] = '-';
                if ((bits & S_IXGRP) != 0)
                    perm[6] = 'x';
                else
                    perm[6] = '-';
                if ((bits & S_IROTH) != 0)
                    perm[7] = 'r';
                else
                    perm[7] = '-';
                if ((bits & S_IWOTH) != 0)
                    perm[8] = 'w';
                else
                    perm[8] = '-';
                if ((bits & S_IXOTH) != 0)
                    perm[9] = 'x';
                else
                    perm[9] = '-';
                perm[10] = '\0';

                struct passwd *pw1;
                pw1 = getpwuid(statres.st_uid);
                struct passwd *pw2;
                pw2 = getpwuid(statres.st_gid);

                struct tm lt;
                time_t t = statres.st_mtime;
                localtime_r(&t, &lt);
                char month[80];
                strftime(month, sizeof(month), "%b", &lt);
                char min[80];
                strftime(min, sizeof(min), "%M", &lt);
                char day[80];
                strftime(day, sizeof(day), "%d", &lt);
                char tim[80];
                strftime(tim, sizeof(tim), "%H", &lt);

                printf("%s %lu\t%s\t%s\t%ld\t%s %s %s:%s %s\n", perm, statres.st_nlink, pw1->pw_name, pw2->pw_name, statres.st_size, month, day, tim, min, filname);
            }
        }
        if (a_flag == 1)
        {
            char *filname = "..";
            if (strcmp(curdir, "/") != 0)
            {
                cd(home, "..");
                char backdir[1024];
                getcwd(backdir, sizeof(newdir));
                cd(home, curdir);

                if (stat(backdir, &statres) >= 0)
                {
                    mode_t bits = statres.st_mode;
                    char perm[11];
                    if (S_ISREG(statres.st_mode))
                        perm[0] = '-';
                    else
                        perm[0] = 'd';
                    if ((bits & S_IRUSR) != 0)
                        perm[1] = 'r';
                    else
                        perm[1] = '-';
                    if ((bits & S_IWUSR) != 0)
                        perm[2] = 'w';
                    else
                        perm[2] = '-';
                    if ((bits & S_IXUSR) != 0)
                        perm[3] = 'x';
                    else
                        perm[3] = '-';
                    if ((bits & S_IRGRP) != 0)
                        perm[4] = 'r';
                    else
                        perm[4] = '-';
                    if ((bits & S_IWGRP) != 0)
                        perm[5] = 'w';
                    else
                        perm[5] = '-';
                    if ((bits & S_IXGRP) != 0)
                        perm[6] = 'x';
                    else
                        perm[6] = '-';
                    if ((bits & S_IROTH) != 0)
                        perm[7] = 'r';
                    else
                        perm[7] = '-';
                    if ((bits & S_IWOTH) != 0)
                        perm[8] = 'w';
                    else
                        perm[8] = '-';
                    if ((bits & S_IXOTH) != 0)
                        perm[9] = 'x';
                    else
                        perm[9] = '-';
                    perm[10] = '\0';

                    struct passwd *pw1;
                    pw1 = getpwuid(statres.st_uid);
                    struct passwd *pw2;
                    pw2 = getpwuid(statres.st_gid);

                    struct tm lt;
                    time_t t = statres.st_mtime;
                    localtime_r(&t, &lt);
                    char month[80];
                    strftime(month, sizeof(month), "%b", &lt);
                    char min[80];
                    strftime(min, sizeof(min), "%M", &lt);
                    char day[80];
                    strftime(day, sizeof(day), "%d", &lt);
                    char tim[80];
                    strftime(tim, sizeof(tim), "%H", &lt);

                    printf("%s %lu\t%s\t%s\t%ld\t%s %s %s:%s %s\n", perm, statres.st_nlink, pw1->pw_name, pw2->pw_name, statres.st_size, month, day, tim, min, filname);
                }
            }
        }
        for (i = 1; i < count + 1; ++i)
        {
            char *filname = files[i - 1]->d_name;
            for (int j = 0; j < strlen(filname); j++)
            {
                x[j + strlen(newdir)] = filname[j];
            }
            x[strlen(newdir) + strlen(filname)] = '\0';
            // printf("%s\n", x);
            if (a_flag == 1 || filname[0] != '.')
            {
                if (stat(x, &statres) >= 0)
                {
                    mode_t bits = statres.st_mode;
                    char perm[11];
                    if (S_ISREG(statres.st_mode))
                        perm[0] = '-';
                    else
                        perm[0] = 'd';
                    if ((bits & S_IRUSR) != 0)
                        perm[1] = 'r';
                    else
                        perm[1] = '-';
                    if ((bits & S_IWUSR) != 0)
                        perm[2] = 'w';
                    else
                        perm[2] = '-';
                    if ((bits & S_IXUSR) != 0)
                        perm[3] = 'x';
                    else
                        perm[3] = '-';
                    if ((bits & S_IRGRP) != 0)
                        perm[4] = 'r';
                    else
                        perm[4] = '-';
                    if ((bits & S_IWGRP) != 0)
                        perm[5] = 'w';
                    else
                        perm[5] = '-';
                    if ((bits & S_IXGRP) != 0)
                        perm[6] = 'x';
                    else
                        perm[6] = '-';
                    if ((bits & S_IROTH) != 0)
                        perm[7] = 'r';
                    else
                        perm[7] = '-';
                    if ((bits & S_IWOTH) != 0)
                        perm[8] = 'w';
                    else
                        perm[8] = '-';
                    if ((bits & S_IXOTH) != 0)
                        perm[9] = 'x';
                    else
                        perm[9] = '-';
                    perm[10] = '\0';

                    struct passwd *pw1;
                    pw1 = getpwuid(statres.st_uid);
                    struct passwd *pw2;
                    pw2 = getpwuid(statres.st_gid);

                    struct tm lt;
                    time_t t = statres.st_mtime;
                    localtime_r(&t, &lt);
                    char month[80];
                    strftime(month, sizeof(month), "%b", &lt);
                    char min[80];
                    strftime(min, sizeof(min), "%M", &lt);
                    char day[80];
                    strftime(day, sizeof(day), "%d", &lt);
                    char tim[80];
                    strftime(tim, sizeof(tim), "%H", &lt);

                    printf("%s %lu\t%s\t%s\t%ld\t%s %s %s:%s %s\n", perm, statres.st_nlink, pw1->pw_name, pw2->pw_name, statres.st_size, month, day, tim, min, filname);
                }
            }
        }
    }
    // printf("\n"); /* flush buffer */
}