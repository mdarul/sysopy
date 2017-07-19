#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "info.h"

// i know it's painful, i use it in order to have size limit
size_t max_size = 0;

void help();
int nftw_visitor(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);


int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        printf("Incorrect number of arguments!\n");
        help();
        abort();
    }

    max_size = (size_t) atoi(argv[2]);
    nftw(argv[1], nftw_visitor, 32, FTW_PHYS);

    return 0;
}


void help()
{
    printf("Usage:\tprogram <directory> <max size>\n");
}

int nftw_visitor(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if ((intmax_t) sb->st_size <= max_size) {
        show_info(fpath, sb);
    }

    return 0;
}