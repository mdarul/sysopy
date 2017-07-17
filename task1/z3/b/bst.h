//
// Created by michal on 09/07/17.
//

#ifndef TASK1_BST_H
#define TASK1_BST_H

#include "contact.h"

typedef struct BSTnode {
    struct BSTnode *left, *right, *parent;
    Contact *contact;
} BSTnode;

typedef struct BST {
    BSTnode *root;
    ContactField key; // current sorting key
} BST;

BST *create_BST(ContactField);
void insert_node_to_BST(BSTnode **, BSTnode *, ContactField, Contact *);
void print_BST(BSTnode *);
BST *sort_BST(BST *, ContactField);
void remove_node_from_BST(BST *, BSTnode *);
BSTnode *find_min_BST(BSTnode *);
BSTnode *find_successor(BSTnode *);
void delete_bst(BST **);
Contact *find_node_in_bst(BST *, Contact *);

#endif //TASK1_BST_H