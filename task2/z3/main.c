#include <stdio.h>
#include <memory.h>
#include <fcntl.h>

#include "lock.h"

void run_interactive_mode(int file_descriptor);

int main(int argc, char *argv[])
{
    int file_descriptor = open(argv[1],O_RDWR);
    if(file_descriptor == -1){
        perror(argv[1]);
        return 1;
    }
    run_interactive_mode(file_descriptor);
    return 0;
}

void run_interactive_mode(int file_descriptor)
{
    int flocks_max = 4;
    int flocks_count = 0;
    struct flock_info flock_table[flocks_max];
    for(int i=0; i<flocks_max; i++)
    {
        flock_table[i].type = F_FREE;
        flock_table[i].index = -1;
        flock_table[i].pid = -1;
    }
    long int a;
    while(1)
    {
        char buf[100];
        scanf("%s",buf);

        if(strcmp(buf, "help") == 0 || strcmp(buf, "h")==0)
        {
            printf("Commends:\n"
                           "r        - read byte from file\n"
                           "w        - write byte to file\n"
                           "wl       - lock writing for specific byte\n"
                           "wlw      - runs wl, but it waits till lock is released (if the lock is held)\n"
                           "wr       - lock reading\n"
                           "wrw      - just like wlw, but with wr\n"
                           "s        - print all locks\n"
                           "h, help  - help (some sort of recursion, huh?)\n"
                           "q        - quit\n");
            continue;
        }

        if(strcmp(buf, "r") == 0)
        {
            printf("Index of reading character: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_RDLCK) != -1)
            {
                printf("Cannot read the character, its' byte is locked!\n");
                continue;
            }
            read_byte(file_descriptor, a);
            continue;
        }

        if(strcmp(buf, "w") == 0)
        {
            char c;
            printf("Index of writing character: ");
            scanf("%ld %c", &a, &c);
            write_byte(file_descriptor,a,c);
            continue;
        }

        if(strcmp(buf, "wl") == 0)
        {
            if(flocks_count == flocks_max - 1)
            {
                printf("Cannot create new lock, too many locks already\n");
                continue;
            }
            printf("Index of locking character: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_WRLCK) != -1)
            {
                printf("Lock already exists!\n");
                continue;
            }
            flock_table[find_lock(flock_table, flocks_max, -1, F_FREE)] = set_lock(file_descriptor, a, F_WRLCK, 0);
            flocks_count++;
            continue;
        }

        if(strcmp(buf, "wr") == 0)
        {
            if(flocks_count == flocks_max - 1)
            {
                printf("Cannot create new lock, too many locks already\n");
                continue;
            }
            printf("Index of locking character: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_RDLCK) != -1)
            {
                printf("Lock already exists!\n");
                continue;
            }
            flock_table[find_lock(flock_table, flocks_max, -1, F_FREE)] = set_lock(file_descriptor, a, F_RDLCK, 0);
            flocks_count++;
            continue;
        }

        if(strcmp(buf, "wlw") == 0)
        {
            if(flocks_count == flocks_max - 1)
            {
                printf("Cannot create new lock, too many locks already\n");
                continue;
            }
            printf("Index of locking character: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_WRLCK) != -1)
            {
                printf("Lock already exists!\n");
                continue;
            }
            flock_table[find_lock(flock_table, flocks_max, -1, F_FREE)] = set_lock(file_descriptor, a, F_WRLCK, 1); // rind_lock() returns first free element
            flocks_count++;
            continue;
        }

        if(strcmp(buf, "wrw") == 0)
        {
            if(flocks_count == flocks_max - 1)
            {
                printf("Cannot create new lock, too many locks already\n");
                continue;
            }
            printf("Index of locking character: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_RDLCK) != -1)
            {
                printf("Lock already exists!\n");
                continue;
            }
            flock_table[find_lock(flock_table, flocks_max, -1, F_FREE)] = set_lock(file_descriptor, a, F_RDLCK, 1);
            flocks_count++;
            continue;
        }

        if(strcmp(buf, "remove") == 0)
        {
            printf("Offset of removing lock: ");
            scanf("%ld", &a);
            if(find_lock(flock_table, flocks_max, a, F_WRLCK) == -1 && find_lock(flock_table, flocks_max, a, F_RDLCK) == -1)
            {
                printf("Nothing to remove!\n");
                continue;
            }
            remove_lock(file_descriptor, a, flock_table, flocks_max, &flocks_count);
            printf("Removed lock\n");
            continue;
        }

        if(strcmp(buf, "s") == 0)
        {
            print_locks(flock_table, flocks_max);
            continue;
        }

        if(strcmp(buf, "q") == 0)
        {
            return;
        }
    }
}
