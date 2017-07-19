//
// Created by michal on 19/07/17.
//

#ifndef Z3_LOCK_H
#define Z3_LOCK_H

#define F_FREE 2137

typedef struct flock_info
{
    long int index;
    short type;
    int pid;
} flock_info;

void read_byte(int file_descriptor, long int offset);
void write_byte(int file_descriptor, long int offset, char c);

int find_lock(struct flock_info *flock_table, int flocks_max, long int offset, short lock_type);
void print_locks(struct flock_info *flock_table, int flocks_max);
struct flock_info set_lock(int file_descriptor, long int offset, short lock_type, int wait);
int remove_lock(int file_descriptor, long int offset, struct flock_info *flock_table, int flocks_max, int *flocks_count);

#endif //Z3_LOCK_H
