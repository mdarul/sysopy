#include <ftw.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "info.h"


void help();
void explore_dir(const char *path, size_t max_size);


int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        printf("Incorrect number of arguments!\n");
        help();
        abort();
    }

    explore_dir(argv[1], (size_t) atoi(argv[2]));

    return 0;
}


void help()
{
    printf("Usage:\tprogram <directory> <max size>\n");
}

void explore_dir(const char *path, size_t max_size)
{
    char real_path[PATH_MAX];
    if (!realpath(path, real_path))
    {
        printf("failed to determine real path of %s\n", path);
        return;
    }

    struct stat file_info;

    if (lstat(real_path, &file_info) == -1)
    {
        printf("Cannot get file_info(stat function) for %s", real_path);
        return;
    }

    if (S_ISDIR(file_info.st_mode)) // if file is dir
    {
        DIR *dir = opendir(real_path);
        if (dir == NULL)
        {
            printf("failed to open %s\n", real_path);
            return;
        }

        struct dirent *dir_element;
        while ((dir_element = readdir(dir)) != NULL)
        {
            if(strcmp(dir_element->d_name, ".") != 0 && strcmp(dir_element->d_name, "..") != 0)
            {
                char child_file_name[PATH_MAX];
                snprintf(child_file_name, PATH_MAX, "%s/%s", real_path, dir_element->d_name);
                explore_dir(child_file_name, max_size);
            }
        }

        closedir(dir);
    }
    else if (S_ISREG(file_info.st_mode) && (intmax_t) file_info.st_size <= max_size) // if is regular file and size is lower/equal
        show_info(real_path, &file_info);

}