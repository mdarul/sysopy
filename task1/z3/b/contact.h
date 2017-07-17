//
// Created by michal on 09/07/17.
//

#ifndef TASK1_CONTACT_H
#define TASK1_CONTACT_H

typedef struct Date {
    char *day;
    char *month;
    char *year;
} Date;

typedef struct Contact {
    char *first_name;
    char *last_name;
    Date birth_date;
    char *email;
    char *phone_number;
    char *address;
} Contact;

typedef enum ContactField {
    FIRSTNAME,
    LASTNAME,
    DAY,
    MONTH,
    YEAR,
    EMAIL,
    PHONENUMBER,
    ADDRESS
} ContactField;

char *get_field(Contact *contact, ContactField field);
void load_contacts(Contact **);
void print_contact(Contact *);

#endif //TASK1_CONTACT_H
