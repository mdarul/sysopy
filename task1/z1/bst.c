//
// Created by michal on 09/07/17.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bst.h"

BST *create_BST(ContactField key)
{
    BST *bst = (BST *) malloc(sizeof(BST));
    bst->root = NULL;
    bst->key = key;
    return bst;
}

void insert_node_to_BST(BSTnode **root, BSTnode *parent, ContactField key, Contact *contact)
{
    if (*root == NULL) {
        *root = (BSTnode *) malloc(sizeof(BSTnode));
        (*root)->parent = parent;
        (*root)->left = (*root)->right = NULL;
        (*root)->contact = contact;
    }
    else if (strcmp(get_field(contact, key), get_field((*root)->contact, key)) < 0) insert_node_to_BST(&((*root)->left), *root, key, contact);
    else insert_node_to_BST(&((*root)->right), *root, key, contact);
}

void print_BST(BSTnode *node)
{
    if(node) {
        print_BST(node->left);
        print_contact(node->contact);
        print_BST(node->right);
    }
}

BSTnode *find_min_BST(BSTnode *node)
{
    while(node->left != NULL) node = node->left;
    return node;
}

BSTnode *find_successor(BSTnode *root)
{
    if(root->right != NULL)return find_min_BST(root);
    else
    {
        BSTnode *tmp = root->parent;
        while(tmp != NULL && tmp->right == root)
        {
            root = tmp;
            tmp = root->parent;
        }
        return tmp;
    }
}

//Contact *remove_node_from_BST(BST *tree, BSTnode *node)
//{
//    Contact *result = node->contact;
//    if (node->left && node->right) {
//        node->contact = remove_node_from_BST(tree, find_successor(node));
//    } else {
//        BSTnode *child = (node->left) ? node->left : node->right;
//        if (child)
//            child->parent = node->parent;
//        if (tree->root == node)
//            tree->root = child;
//        else if (node->parent->left == node)
//            node->parent->left = child;
//        else
//            node->parent->right = child;
//        free(node);
//    }
//    return result;
//}
//
//BST *sort_BST(BST *bst, ContactField key)
//{
//    BST *sorted = create_BST(key);
//    while(bst->root != NULL) {
//        insert_node_to_BST(&(sorted->root), NULL, sorted->key, remove_node_from_BST(bst, bst->root));
//    }
//
//    return sorted;
//}


void remove_node_from_BST(BST *tree, BSTnode *node)
{
    if (node->left && node->right) {
        node->contact = find_successor(node)->contact;
        remove_node_from_BST(tree, find_successor(node));
    } else {
        BSTnode *child = (node->left) ? node->left : node->right;
        if (child)
            child->parent = node->parent;
        if (tree->root == node)
            tree->root = child;
        else if (node->parent->left == node)
            node->parent->left = child;
        else
            node->parent->right = child;
        free(node);
    }
}

BST *sort_BST(BST *bst, ContactField key)
{
    BST *sorted = create_BST(key);
    Contact *contact_buffer;
    while(bst->root != NULL) {
        contact_buffer = bst->root->contact;
        remove_node_from_BST(bst, bst->root);
        insert_node_to_BST(&(sorted->root), NULL, sorted->key, contact_buffer);
    }
    return sorted;
}

void delete_bst(BST **bst)
{
    while((*bst)->root)remove_node_from_BST((*bst), (*bst)->root);
    free((*bst));
    (*bst) = NULL;
}

Contact *find_node_in_bst(BST *bst, Contact *contact)
{
    BSTnode *tmp = bst->root;
    while(tmp)
    {
        if(tmp->contact == contact)return tmp->contact;
        if(strcmp(get_field(contact, bst->key), get_field(tmp->contact, bst->key))>0) tmp = tmp->right;
        else tmp = tmp->left;
    }
    return NULL;
}