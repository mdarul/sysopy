//
// Created by michal on 12/07/17.
//

#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DLlist *create_list(ContactField key)
{
    DLlist *list = (DLlist *) malloc(sizeof(DLlist));
    list->key = key;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void insert_contact_to_list(DLnode **first, DLnode **last, ContactField key, Contact *contact)
{
    DLnode *new_node = (DLnode *) malloc(sizeof(DLlist));
    new_node->contact = contact;
    new_node->prev = new_node->next = NULL;

    if((*first)==NULL) (*first) = (*last) = new_node;
    else
    {
        //add at the beginning
        if(strcmp(get_field(new_node->contact, key), get_field((*first)->contact, key)) <= 0)
        {
            new_node->next = (*first);
            (*first)->prev = new_node;
            (*first) = new_node;
        }
        //add at the end
        else if(strcmp(get_field(new_node->contact, key), get_field((*last)->contact, key)) > 0)
        {
            new_node->prev = (*last);
            (*last)->next = new_node;
            (*last) = new_node;
        }
        else
        {
            DLnode *tmp_first = (*first)->next;
            DLnode *previous=(*first);
            while(tmp_first && strcmp(get_field(new_node->contact, key), get_field(tmp_first->contact, key)) >= 0)
            {
                previous = tmp_first;
                tmp_first = tmp_first->next;
            }
            new_node->next = tmp_first;
            new_node->prev = previous;
            previous->next = new_node;
            if(tmp_first) tmp_first->prev = new_node;
        }
    }
}

void remove_node_from_list(DLlist **list, DLnode *node)
{
    //one-node list
    if(node == (*list)->first && node == (*list)->last)
    {
        (*list)->first = (*list)->last = NULL;
        free(node);
    }
    //remove from the begining
    else if(node == (*list)->first)
    {
        (*list)->first = (*list)->first->next;
        (*list)->first->prev = NULL;
        free(node);
    }
    //remove from the end
    else if(node == (*list)->last)
    {
        (*list)->last -> next = NULL;
        (*list)->last = (*list)->last->prev;
        free(node);
    }
    else
    {
        DLnode *tmp = (*list)->first;
        while(tmp != node) tmp = tmp->next;
        if(tmp) //if there is none element 'node' in the list, we will not continue
        {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            free(node);
        }
    }
}

DLlist *sort_list(DLlist *list, ContactField key)
{
    DLlist *new_list = create_list(key);
    Contact *tmp_contact;

    while(list->first)
    {
        tmp_contact = list->first->contact;
        remove_node_from_list(&list, list->first);
        insert_contact_to_list(&(new_list)->first, &(new_list)->last, new_list->key, tmp_contact);
    }
    return new_list;
}

void print_list(DLlist *list)
{
    DLnode *tmp = list->first;
    while(tmp != NULL)
    {
        print_contact(tmp->contact);
        tmp = tmp->next;
    }
}

void print_list_backwards(DLlist *list)
{
    DLnode *tmp = list->last;
    while(tmp != NULL)
    {
        print_contact(tmp->contact);
        tmp = tmp->prev;
    }
}

void delete_list(DLlist **list)
{
    while((*list)->first) remove_node_from_list(&(*list), (*list)->last);
    free((*list));
    (*list) = NULL;
}

Contact *find_node_in_list(DLlist *list, Contact *contact)
{
    DLnode *tmp = list->first;
    while(tmp)
    {
        if(tmp->contact == contact)return tmp->contact;
        tmp = tmp->next;
    }
    return NULL;
}