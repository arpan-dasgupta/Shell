#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>

extern int alphasort();

#define clear() printf("\033[H\033[J")

int file_select(const struct dirent *entry)
{
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) || (strcmp(entry->d_name, "~") == 0))
        return 0;
    else
        return 1;
}

void listfiles(char *dir)
{
    struct dirent **files;
    int count, i;
    count = scandir(dir, &files, file_select, alphasort);
    printf("Number of files = %d\n", count);
    for (i = 1; i < count + 1; ++i)
        printf("%s\n", files[i - 1]->d_name);
    printf("\n"); /* flush buffer */
}