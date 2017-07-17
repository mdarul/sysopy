//
// Created by michal on 09/07/17.
//

#include "contact.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *get_field(Contact *contact, ContactField field)
{
    switch(field)
    {
        case FIRSTNAME:
            return contact->first_name;
        case LASTNAME:
            return contact->last_name;
        case DAY:
            return contact->birth_date.day;
        case MONTH:
            return contact->birth_date.month;
        case YEAR:
            return contact->birth_date.year;
        case EMAIL:
            return contact->email;
        case PHONENUMBER:
            return contact->phone_number;
        case ADDRESS:
            return contact->address;
    }
    //else
    return NULL;
}

char *get_substring(char *source, int a, int b)
{
    char *destination = malloc(sizeof(char) * (b-a+3));
    int i;
    for(i=0; i<=(b-a) && source[a+i]!=0; i++)destination[i] = source[a+i];
    destination[i+1] = '\0';
    return destination;
}

void load_contacts(Contact **contacts)
{
    int contact_number=0;
    FILE * fp;
    char * line = NULL;
    size_t len = 0;

    fp = fopen("data.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int commas_positions[5];
    int j;
    while (getline(&line, &len, fp) != -1)
    {
        for(j=0; j<5; j++) commas_positions[j] = 0;
        int i, comma_number=0;
        for(i=0; i<strlen(line); i++)
        {
            if(line[i] == ',')
            {
                commas_positions[comma_number] = i;
                comma_number++;
            }
        }

        Contact *contact = (Contact *) malloc(sizeof(Contact));

        contact->first_name = get_substring(line, 0, commas_positions[0]-1);
        contact->last_name = get_substring(line, commas_positions[0]+1, commas_positions[1]-1);
        contact->email = get_substring(line, commas_positions[2]+1, commas_positions[3]-1);
        contact->phone_number = get_substring(line, commas_positions[3]+1, commas_positions[4]-1);
        contact->address = get_substring(line, commas_positions[4]+1, strlen(line)-2);
        contact->birth_date.day = get_substring(line, commas_positions[1]+1, commas_positions[1]+2);
        contact->birth_date.month = get_substring(line, commas_positions[1]+4, commas_positions[1]+5);
        contact->birth_date.year = get_substring(line, commas_positions[1]+7, commas_positions[1]+10);

        contacts[contact_number++] = contact;
    }

    fclose(fp);
    if (line)
        free(line);
}

void print_contact(Contact *contact)
{
    printf("|%s|%s|%s|%s|%s|%s|%s|%s|\n",
           contact->first_name,
           contact->last_name,
           contact->birth_date.day,
           contact->birth_date.month,
           contact->birth_date.year,
           contact->email,
           contact->phone_number,
           contact->address);
}
