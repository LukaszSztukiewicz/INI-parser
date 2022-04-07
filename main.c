/*
==============================================
                INI PARSER
Input formats:
 1. ./program PATH_TO_INI expression "section.key+section2.key2"
 2. ./program PATH_TO_INI section.key

 Structure of ini save:
    [section] -> [key]

Mode 1 - expression evaluation
Mode 2 - displaying value of section.key
==============================================
*/

#include <ctype.h>
#include <stdbool.h>
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

// utility functions
bool is_identifier_valid(char *identifier) {
  int length = strlen(identifier);
  for (int i = 0; i < length; i++) {
    if (!isalnum(identifier[i]) && identifier[i] != '-') {
      return false;
    }
  }
  return true;
}
bool is_integer_value(char *value) {
  int length = strlen(value);
  for (int i = 0; i < length; i++) {
    if (!isdigit(value[i])) {
      return false;
    }
  }
  return true;
}
void free_mem(struct Section *first_section) {
  // traverse all sections
  while (first_section != NULL) {
    free(first_section->name);
    // traverse all keys in section
    while (first_section->keys != NULL) {
      free(first_section->keys->key);
      free(first_section->keys->value);
      struct Key *key_to_free = first_section->keys;
      first_section->keys     = first_section->keys->nextkey;
      free(key_to_free);
    }
    struct Section *section_to_free = first_section;
    first_section                   = first_section->nextsection;
    free(section_to_free);
  }
  free(first_section);
}

// define parse function returning pointer to first struct section
struct Section *parse_file(FILE *file) {
  struct Section *first_section = NULL;
  char buffer[1024];
  while (fgets(buffer, 1024, file) != NULL) {
    if (buffer[0] == '[') {
      // create a new section
      char *section_name = strtok(buffer, "]") + 1;
      if (!is_identifier_valid(section_name)) {
        printf("Invalid section identifier \"%s\" in INI file\n", section_name);
      }
      struct Section *new_section = malloc(sizeof(struct Section));
      new_section->name           = malloc(sizeof(char) * strlen(section_name) + 1);
      strcpy(new_section->name, section_name);
      new_section->keys        = NULL;
      new_section->nextsection = NULL;
      if (first_section == NULL) {
        first_section = new_section;
      } else {
        // traverse sections to return pointer to last section
        struct Section *i_section = first_section;
        while (i_section->nextsection != NULL) {
          i_section = i_section->nextsection;
        }
        i_section->nextsection = new_section;
      }
    } else if (buffer[0] == '\n') {
      continue;
    } else {

      // create a new key
      char *key = strtok(buffer, " ");
      strtok(NULL, " ");
      char *value = strtok(NULL, "\n");
      if (!is_identifier_valid(key)) {
        printf("Invalid key identifier \"%s\" in INI file\n", key);
      }
      struct Key *new_key = malloc(sizeof(struct Key));
      new_key->key        = malloc(sizeof(char) * strlen(key) + 1);
      strcpy(new_key->key, key);
      new_key->value = malloc(sizeof(char) * strlen(value) + 1);
      strcpy(new_key->value, value);
      new_key->nextkey = NULL;

      // traverse sections to return pointer to last section
      struct Section *i_section = first_section;
      while (i_section->nextsection != NULL) {
        i_section = i_section->nextsection;
      }
      // if section doesnt have keys yet, add key to section
      if (i_section->keys == NULL) {
        i_section->keys = new_key;
      }
      // traverse keys to return pointer to last key
      else {
        struct Key *i_key = i_section->keys;
        while (i_key->nextkey != NULL) {
          i_key = i_key->nextkey;
        }
        i_key->nextkey = new_key;
      }
    }
  }

  return first_section;
}

char *read_value_from_section(struct Section *first_section, char *section, char *key) {

  // traverse sections
  struct Section *i_section = first_section;
  while (strcmp(i_section->name, section) != 0) {
    if (i_section->nextsection == NULL)
      return "Error:section-not-found";
    i_section = i_section->nextsection;
  }

  // traverse keys
  struct Key *i_key = i_section->keys;
  while (strcmp(i_key->key, key) != 0) {
    if (i_key->nextkey == NULL)
      return "Error:key-not-found";
    i_key = i_key->nextkey;
  }

  return i_key->value;
}

char *parse_expression(struct Section *parsed_sections, char *expression) {
  // parse expression in quotes to variables
  char *first_argument_section  = strtok(expression, " .");
  char *first_argument_key      = strtok(NULL, " .");
  char *first_value             = read_value_from_section(parsed_sections, first_argument_section, first_argument_key);
  char *operand                 = strtok(NULL, " .");
  char *second_argument_section = strtok(NULL, " .");
  char *second_argument_key     = strtok(NULL, " .");
  char *second_value            = read_value_from_section(parsed_sections, second_argument_section, second_argument_key);
  bool is_int_first_value       = is_integer_value(first_value);
  bool is_int_second_value      = is_integer_value(second_value);
  char *message                 = malloc(sizeof(char) * (strlen(first_value) + strlen(second_value)) + 1);

  if (is_int_first_value && is_int_second_value) {

    // perform operations on integer values
    // long type is required since multiplication operation can be also performed
    long first_int_value  = atol(first_value);
    long second_int_value = atol(second_value);
    switch (*operand) {
    case '+':
      printf("%ld", first_int_value + second_int_value);
      break;
    case '-':
      printf("%ld", first_int_value - second_int_value);
      break;
    case '*':
      printf("%ld", first_int_value * second_int_value);
      break;
    case '/':
      printf("%ld", first_int_value / second_int_value);
      break;
    }
    strcpy(message, "");
  }

  // check if the values have same type
  else if ((is_int_first_value && !is_int_second_value) || (!is_int_first_value && is_int_second_value)) {
    strcpy(message, "Error:invalid-operation type mismatch");
  }

  // both values must be strings, so perform operation on strings
  else {
    // only concatenation is allowed
    if (*operand != '+') {
      strcpy(message, "Error:invalid-operation on string");
    } else {
      strcpy(message, first_value);
      strcat(message, second_value);
    }
  }
  return message;
}

int main(int argc, char *argv[]) {
  char *path = argv[1];

  // read file
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found\n");
    return 1;
  }
  struct Section *first_section = parse_file(file);
  fclose(file);

  // Mode 1
  if ((strcmp(argv[2], "expression") == 0)) {

    // in this mode there must be exactly 3 arguments
    if (argc != 4) {
      printf("Bad number of arguments\n");
      free_mem(first_section);
      return 1;
    }
    char *result = parse_expression(first_section, argv[3]);
    printf("%s", result);
  }

  // Mode2
  else {

    // parse section and key identifiers
    char *desired_section = strtok(argv[2], ".");
    char *desired_key     = strtok(NULL, ".");

    // perform read operation
    char *result = read_value_from_section(first_section, desired_section, desired_key);

    // process the result
    if (strcmp("Error:section-not-found", result) == 0) {
      printf("Failed to find section [%s]", desired_section);
      free_mem(first_section);
      return 1;
    } else if (strcmp("Error:key-not-found", result) == 0) {
      printf("Failed to find key \"%s\" in section [%s]", desired_key, desired_section);
      free_mem(first_section);
      return 1;
    } else {
      printf("%s", result);
    }
  }
  free_mem(first_section);
  return 0;
}