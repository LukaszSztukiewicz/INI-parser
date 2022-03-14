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
struct Key{
    char *key;
    char *value;
    struct Key *nextkey;
};
struct Section {
    char *name;
    struct Key *keys;
    struct Section *nextsection;
};

struct Key *firstKey(){
    struct Key *firstKey = (struct Key *)malloc(sizeof(struct Key));
    struct Key *nextKey = firstKey()
    while(fgets(buf, sizeof buf, file) != NULL){
                if(buf[0] == '['){
                    return firstKey;
                }
            }
    firstKey->key = key;
    firstKey->value = value;
    firstKey->nextkey = NULL;
    return firstKey;
}
//define parse function returning pointer to struct section
struct section *parse_file(FILE *file){
    char *buf = NULL;
    struct section *sections = NULL;
    while(fgets(buf, sizeof buf, file) != NULL){
        if(buf[0] == '['){
            //section
            struct Section *section = malloc(sizeof(struct Section));
            section->name = strtok(buf, "]");
            section->keys = NULL;
            section->nextsection = NULL;
            struct Section *lastsection = section;
            
        }
    }
    return sections;
    
}

int main(int argc, char *argv[]){
    char *path = argv[1];
    char *state = argv[2];
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("File not found\n");
        return NULL;
    }
    struct Section *parsed_file = parse_file(file);
    if(*state == 'expression'){
        char *expression = argv[3];
    }
    else{
        
    }

    
    

    return 0;
}