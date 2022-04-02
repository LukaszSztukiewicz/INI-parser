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
#include <string.h>

int main(int argc, char *argv[])
{
    char *desired_section = strtok(argv[1], ".");
    printf("%s", desired_section);
    desired_section = strtok(NULL, ".");
    printf("%s", desired_section);
    return 0;
}