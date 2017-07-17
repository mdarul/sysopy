//
// Created by michal on 12/07/17.
//

#ifndef TASK1_LIST_H
#define TASK1_LIST_H

#include "contact.h"

typedef struct DLnode {
    struct DLnode *prev, *next;
    Contact *contact;
} DLnode;

typedef struct DLlist {
    struct DLnode *first, *last;
    ContactField key;
} DLlist;

DLlist *create_list(ContactField);
void insert_contact_to_list(DLnode **, DLnode **, ContactField, Contact *);
void print_list(DLlist *);
void print_list_backwards(DLlist *);
void remove_node_from_list(DLlist **, DLnode *);
DLlist *sort_list(DLlist *, ContactField);
void delete_list(DLlist **);
Contact *find_node_in_list(DLlist *, Contact *);

#endif //TASK1_LIST_H
