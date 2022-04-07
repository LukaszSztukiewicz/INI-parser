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

#include <bool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Key {
  char *key;
  char *value;
  struct Key *nextkey;
};

struct Section {
  char *name;
  struct Key *keys;
  struct Section *nextsection;
};

// define parse function returning pointer to struct section
struct Section *parse_file(FILE *file) {
  struct Section *first_section = malloc(sizeof(struct Section));
  first_section->name           = NULL;
  first_section->keys           = NULL;
  first_section->nextsection    = NULL;
  char buffer[1024];
  while (fgets(buffer, 1024, file) != NULL) {
    if (buffer[0] == '[') {
      char *section_name = buffer;
      section_name       = strtok(section_name, "]");
      if (is_identifier_valid(key)) {
        struct Section *new_section = malloc(sizeof(struct Section));
        new_section->name           = malloc(sizeof(char) * strlen(section_name));
        strcpy(new_section->name, section_name + 1);
        new_section->keys        = NULL;
        new_section->nextsection = NULL;
        if (first_section->name == NULL) {
          first_section = new_section;
        } else {
          struct Section *i_section = first_section;
          while (i_section->nextsection != NULL) {
            i_section = i_section->nextsection;
          }
          i_section->nextsection = new_section;
        }
      } else
        printf("Invalid section identifier %s in INI file", *section_name);
    } else if (buffer[0] == '\n') {
      continue;
    } else {
      char *key_value = buffer;
      char *key       = strtok(key_value, "=");
      char *value     = strtok(NULL, "\n");
      if (is_identifier_valid(key)) {
        struct Key *new_key  = malloc(sizeof(struct Key));
        new_key->key         = NULL;
        new_key->value       = NULL;
        new_key->nextkey     = NULL;
        new_key->key         = malloc(sizeof(char) * strlen(key));
        key[strlen(key) - 1] = '\0';
        strcpy(new_key->key, key);
        new_key->value = malloc(sizeof(char) * strlen(value));
        strcpy(new_key->value, value + 1);
        struct Section *i_section = first_section;
        while (i_section->nextsection != NULL) {
          i_section = i_section->nextsection;
        }
        if (i_section->keys == NULL) {
          i_section->keys = new_key;
        } else {
          struct Key *i_key = i_section->keys;
          while (i_key->nextkey != NULL) {
            i_key = i_key->nextkey;
          }
          i_key->nextkey = new_key;
        }
      } else
        printf("Invalid key identifier %s in INI file", *key);
    }
  }
  return first_section;
}
bool *is_identifier_valid(char *identifier) {
  int length = strlen(identifier);
  for (int i = 0; i < length; i++) {
    if (!isalnum(identifier[i]) && identifier[i] != '-') {
      return false;
    }
  }
  return true;
}
char *parse_expression(char *expression) {
  if (strcmp(validation_result, "int") == 0) {
    int *first_argument  = strtok(expression, " ");
    int *operand         = strtok(NULL, " ");
    int *second_argument = strtok(NULL, " ");
    int *result          = int_expression(first_argument, second_argument, operand);
  } else if (strcmp(validation_result, "string") == 0) {
    char *first_argument         = strtok(expression, " ");
    char *second_argument        = strtok(NULL, " ");
    char *first_argument_section = strtok(first_argument, ".");
    char *first_argument_key     = strtok(NULL, ".");
    int *result                  = *first_argument + *second_argument;

  } else if (strcmp(validation_result, "invalid") == 0) {
    printf("Invalid arguments for expression: %s\n", expression);
    free_mem(first_section);
    return 1;
  } else {
    printf("Unexpected return from argv_validation function: %s\n", validation_result);
    free_mem(first_section);
    return 1;
  }
}

char *read_value_from_section(struct Section *first_section, char *section, char *key) {

  struct Section *i_section = malloc(sizeof(struct Section));
  i_section                 = first_section;
  while (strcmp(i_section->name, section) != 0) {
    if (i_section->nextsection == NULL)
      return "Error:section-not-found";
    i_section = i_section->nextsection;
  }

  struct Key *i_key = malloc(sizeof(struct Key));
  i_key             = i_section->keys;
  while (strcmp(i_key->key, key) != 0) {
    if (i_key->nextkey == NULL)
      return "Error:key-not-found";
    i_key = i_key->nextkey;
  }
  return i_key->value;
}

void free_mem(struct Section *first_section) {
  struct Section *i_section = malloc(sizeof(struct Section));
  i_section                 = first_section;
  while (i_section->nextsection != NULL) {
    struct Key *i_key = malloc(sizeof(struct Key));
    i_key             = i_section->keys;
    while (i_key->nextkey != NULL) {
      free(i_key->key);
      free(i_key->value);
      i_key = i_key->nextkey;
    }
    free(i_section->name);
    free(i_section->keys);
    i_section = i_section->nextsection;
  }
}

int main(int argc, char *argv[]) {
  char *path = argv[1];
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found\n");
    return 1;
  }
  struct Section *first_section = parse_file(file);
  fclose(file);

  // Mode 1
  if ((strcmp(argv[2], "expression") == 0)) {
    // check for proper number of arguments
    if (argc != 4) {
      printf("Bad number of arguments\n");
      free_mem(first_section);
      return 1;
    }
    char *result = parse_expression(argv[3]);
  }

  // Mode2
  else {
    char *desired_section = strtok(argv[2], ".");
    char *desired_key     = strtok(NULL, ".");
    char *result          = read_value_from_section(first_section, desired_section, desired_key);
    if (strcmp("Error:section-not-found", result) == 0) {
      printf("Failed to find section [%s]", desired_section);
      free_mem(first_section);
      return 1;
    } else if (strcmp("Error:key-not-found", result) == 0) {
      printf("Failed to find key \"%s\" in section [%s]", desired_key, desired_section);
      free_mem(first_section);
      return 1;
    } else
      printf("%s", result);
  }

  // free memory
  free_mem(first_section);

  return 0;
}