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

struct Key *firstKey()
{
    /*struct Key *firstKey = malloc(sizeof(struct Key));
    struct Key *nextKey = firstKey();
    while (fgets(buf, sizeof buf, file) != NULL)
    {
        if (buf[0] == '[')
        {
            return firstKey;
        }
    }
    firstKey->key = key;
    firstKey->value = value;
    firstKey->nextkey = NULL;
    return firstKey;*/
}
// define parse function returning pointer to struct section
struct section *parse_file(FILE *file)
{
    char *buf = NULL;
    struct section *sections = NULL;
    while (fgets(buf, sizeof buf, file) != NULL)
    {
        if (buf[0] == '[')
        {
            // section
            struct Section *section = malloc(sizeof(struct Section));
            section->name = strtok(buf, "]");
            section->keys = firstKey();
            section->nextsection = NULL;
            struct Section *lastsection = section;
        }
    }
    return sections;
}
// define parse function returning pointer to struct section
char *argv_validation(FILE *file)
{
    char *buf = NULL;
    struct section *sections = NULL;
    while (fgets(buf, sizeof buf, file) != NULL)
    {
        if (buf[0] == '[')
        {
            // section
            struct Section *section = malloc(sizeof(struct Section));
            section->name = strtok(buf, "]");
            section->keys = firstKey();
            section->nextsection = NULL;
            struct Section *lastsection = section;
        }
    }
    return sections;
}

char *read_value_from_section(char *first_section, char *section, char *key)
{
}

char *get_section(char *state)
{
}

char *get_key(char *state)
{
}

int main(int argc, char *argv[])
{
    char *path = argv[1];
    char *state = argv[2];
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    struct Section *first_section = parse_file(file);

    // handling expression input
    if (*state == 'expression')
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
            printf("Invalid arguments for program: %s\n", expression);
            return 1;
        }
        else
        {
            printf("Unexpected return from argv_validation function: %s\n", validation_result);
            return 1;
        }
    }
    // handling section.key input
    else
    {
        char *desired_section = get_section(state);
        char *desired_key = get_key(state);
        char *result = read_value_from_section(first_section, desired_section, desired_key);
        printf("%s", result);
    }
    return 0;
}