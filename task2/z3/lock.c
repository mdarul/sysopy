//
// Created by michal on 19/07/17.
//

#include "lock.h"

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void read_byte(int file_descriptor, long int offset)
{
    void *buf = malloc(sizeof(char));
    lseek(file_descriptor, offset, SEEK_SET);
    read(file_descriptor, buf, 1);
    printf("%s\n", (char*)buf);
}

void write_byte(int file_descriptor, long int offset, char c)
{
    lseek(file_descriptor, offset, SEEK_SET);
    char buf[1];
    buf[0] = c;
    write(file_descriptor, buf, 1);
}

flock_info set_lock(int file_descriptor, long int offset, short lock_type, int wait)
{
    struct flock lock;
    lock.l_type = lock_type;
    lock.l_whence = SEEK_SET;
    lock.l_start = (size_t) offset;
    lock.l_len = 1;

    if(fcntl(file_descriptor, wait ? F_SETLKW : F_SETLK, &lock) == -1)
    {
        printf("Cannot acquire a lock\n");
        abort();
    }
    else
    {
        printf("Character locked, lock type: %s\n", lock_type == F_RDLCK ? "F_RDLCK" : "F_WRLCK");
        struct flock_info info;
        info.type = lock_type;
        info.index = offset;
        info.pid = (int)lock.l_pid;
        return info;
    }
}

int remove_lock(int file_descriptor, long int offset, struct flock_info *flock_table, int flocks_max, int *flocks_count)
{
    struct flock lock = {
            .l_type = F_UNLCK,
            .l_whence = SEEK_SET,
            .l_start = (size_t)offset,
            .l_len = 1
    };
    if(fcntl(file_descriptor, F_SETLK, &lock) == -1)
    {
        printf("Cannot remove the lock!\n");
        return -1;
    }
    else
    {
        for(int i=0; i<flocks_max; i++)
        {
            if((flock_table)[i].index == offset)
            {
                (flock_table)[i].index = -1;
                (flock_table)[i].type = F_FREE;
                (flock_table)[i].pid = -1;
                (*flocks_count)--;
            }
        }
    }
}

void print_locks(struct flock_info *flock_table, int flocks_max)
{
    for(int i=0; i<flocks_max; i++)
    {
        if(flock_table[i].type != F_FREE)
            printf("locked byte: %-5d lock type: %-11s lock owner PID: %d\n",
                   (int)flock_table[i].index,
                   flock_table[i].type==F_RDLCK ? "F_RDLCK" : "F_WRLCK",
                   flock_table[i].pid);
    }
}

int find_lock(struct flock_info *flock_table, int flocks_max, long int offset, short lock_type)
{
    for(int i=0; i<flocks_max; i++)
    {
        if(flock_table[i].type == lock_type && flock_table[i].index == offset)return i;
    }
    return -1;
}