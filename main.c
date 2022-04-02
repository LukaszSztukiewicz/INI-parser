/*
==============================================
                INI PARSER
Input formats:
 1. ./program PATH_TO_INI expression "section.key+section.key"
 2. ./program PATH_TO_INI section.key

 Structure of ini save:
    [section] -> [keyke]

Mode 1 - expr eval
Mode 2 - section.key - only one,


==============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Key
{
    char *key;
    char *value;
    struct Key *nextkey;
};

struct Section
{
    char *name;
    struct Key *keys;
    struct Section *nextsection;
};

// define parse function returning pointer to struct section
struct Section *parse_file(FILE *file)
{
    char *buf = NULL;
    struct Section *first_section = NULL;
    struct Section *last_section = NULL;
    struct Section *current_section = NULL;

    struct Key *first_key = NULL;
    struct Key *last_key = NULL;
    struct Key *current_key = NULL;

    while (fgets(buf, sizeof buf, file) != NULL)
    {
        if (buf[0] == '[')
        {
            // pass the pointers
            current_section->keys = first_key;

            last_section = current_section;
            current_section = (struct Section *)malloc(sizeof(struct Section));
            last_section->nextsection = current_section;

            current_section->name = strtok(buf, "]"); // FIXME with leading [
            // current_section->keys = firstKey();
            // current_section->nextsection = NULL;
        }

        else if (isspace(buf[0]))
        {
            continue;
        }
        else
        {
            last_key = current_key;
            current_key = (struct Key *)malloc(sizeof(struct Key));
            last_key->nextkey = current_key;

            current_key->key = strtok(buf, "="); // FIXME with leading spaces
            current_key->value = strtok(NULL, "=");
        }
    }
    return first_section;
}

char *argv_validation(FILE *file)
{
}

char *read_value_from_section(struct Section *first_section, char *section, char *key)
{
    struct Section *i_section = first_section;
    while (i_section->name != section)
    {
        if (i_section->nextsection == NULL)
            return "Section not found";
        i_section = i_section->nextsection;
    }

    struct Key *i_key = i_section->keys;
    while (i_key->value != key)
    {
        if (i_key->nextkey == NULL)
            return "Key not found";
        i_key = i_key->nextkey;
    }
    return i_key;
}

int main(int argc, char *argv[])
{
    char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    struct Section *first_section = parse_file(file);

    // Mode 1
    if (argv[2] == 'expression')
    {
        char *expression = argv[3];
        char *validation_result = argv_validation(expression);
        if (validation_result == 'int')
        {
            // to implement
        }
        else if (validation_result == 'string')
        {
            // to implement
        }
        else if (validation_result == 'invalid')
        {
            printf("Invalid arguments for expression: %s\n", expression);
            return 1;
        }
        else
        {
            printf("Unexpected return from argv_validation function: %s\n", validation_result);
            return 1;
        }
    }

    // Mode2
    else
    {
        char *desired_section = strtok(argv[2], ".");
        char *desired_key = strtok(NULL, ".");
        char *result = read_value_from_section(first_section, desired_section, desired_key);
        printf("%s", result);
    }
    return 0;
}