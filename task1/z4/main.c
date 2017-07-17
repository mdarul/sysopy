#include <stdio.h>
#include "bst.h"
#include "list.h"
#include "contact.h"
#include <time.h>
#include <sys/resource.h>

void timeCheckpoint(const char *message);

int main() {
    printf("Hello, World!\n");
    Contact *contacts_list[1000];

    printf("Checkpoints format: \"real:user:system - info\", unit 10^-6 s\n");
    timeCheckpoint("Start program");
    load_contacts(contacts_list);
    timeCheckpoint("Read data from a file");

    BST *bst = create_BST(FIRSTNAME);
    DLlist *list = create_list(FIRSTNAME);


    int i;
    for(i=0; i<1000; i++) {
        insert_node_to_BST(&(bst->root), NULL, bst->key, contacts_list[i]);
        timeCheckpoint("Inserted contact to BST");
        insert_contact_to_list(&(list)->first, &(list)->last, list->key, contacts_list[i]);
        timeCheckpoint("Inserted contact to DLlist");
    }

    bst = sort_BST(bst, LASTNAME);
    timeCheckpoint("Sort BST by last name");

    list = sort_list(list, LASTNAME);
    timeCheckpoint("Sort DLlist by last name");

    find_node_in_list(list, list->first->contact);
    timeCheckpoint("Find contact in list (optimisic)");

    find_node_in_list(list, list->last->contact);
    timeCheckpoint("Find contact in list (pessimistic)");

    find_node_in_bst(bst, bst->root->contact);
    timeCheckpoint("Find contact in BST (optimisic)");

    BSTnode *tmp = bst->root;
    while(tmp->right) tmp = tmp->right;
    find_node_in_bst(bst, tmp->contact);
    timeCheckpoint("Find contact in BST (pessimistic)");

    remove_node_from_list(&list, list->first);
    timeCheckpoint("Remove contact from list (optimistic)");

    remove_node_from_list(&list, list->last);
    timeCheckpoint("Remove contact from list (pessimistic)");

    remove_node_from_BST(bst, tmp);
    timeCheckpoint("Remove contact from BST (optimistic)");

    remove_node_from_BST(bst, bst->root);
    timeCheckpoint("Remove contact from BST (pessimistic)");

    return 0;
}

void timeCheckpoint(const char *message)
{
    static struct timespec previous_real_time;
    // Getting time values
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct timeval user_time = usage.ru_utime;
    struct timeval system_time = usage.ru_stime;
    struct timespec real_time;
    clock_gettime(CLOCK_REALTIME, &real_time);
    // Show info
    printf("%8lld : %8ld : %8ld   ---   %s\n",
           (long long)(real_time.tv_sec-previous_real_time.tv_sec)*1000000+(real_time.tv_nsec-previous_real_time.tv_nsec)/1000,
           user_time.tv_sec*1000000+user_time.tv_usec,
           system_time.tv_sec*1000000+system_time.tv_usec,
           message
    );
    previous_real_time = real_time;
}

