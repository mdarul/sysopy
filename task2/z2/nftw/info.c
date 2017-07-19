//
// Created by michal on 18/07/17.
//

#include "info.h"

void show_info(const char *fpath, const struct stat *sb)
{
    char flag_str[10];
    flag_str[0] = (char) ((S_ISDIR(sb->st_mode)) ? 'd' : '-');
    flag_str[1] = (char) ((sb->st_mode & S_IRUSR) ? 'r' : '-');
    flag_str[2] = (char) ((sb->st_mode & S_IWUSR) ? 'w' : '-');
    flag_str[3] = (char) ((sb->st_mode & S_IXUSR) ? 'x' : '-');
    flag_str[4] = (char) ((sb->st_mode & S_IRGRP) ? 'r' : '-');
    flag_str[5] = (char) ((sb->st_mode & S_IWGRP) ? 'w' : '-');
    flag_str[6] = (char) ((sb->st_mode & S_IXGRP) ? 'x' : '-');
    flag_str[7] = (char) ((sb->st_mode & S_IROTH) ? 'r' : '-');
    flag_str[8] = (char) ((sb->st_mode & S_IWOTH) ? 'w' : '-');
    flag_str[9] = (char) ((sb->st_mode & S_IXOTH) ? 'x' : '-');
    flag_str[10] = '\0';
    printf("%s   %d   %s   %s", fpath, (int)sb->st_size, flag_str, ctime(&sb->st_mtime));
}