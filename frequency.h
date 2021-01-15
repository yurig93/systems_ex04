//
// Created by Yuri Grigorian on 15/01/2021.
//


#ifndef SYSTEMS_EX04_FREQUENCY_H
#define SYSTEMS_EX04_FREQUENCY_H

#include <expat.h>
#include "stdbool.h"

#define CHAR_A_LOWER 'a'
#define CHAR_Z_LOWER 'z'
#define CHAR_SEP ' '
#define CHAR_NULL '\0'
#define CHAR_NEWLINE '\n'
#define NUM_LETTERS ((int)CHAR_Z_LOWER - CHAR_A_LOWER + 1)
#define ARG_REVERSE "r"

typedef struct node {
    char letter;
    long unsigned int count;
    struct node *children[NUM_LETTERS];
} node;

node *allocate_new_node(char cLetter, int iCount);

int release_node(node *npNode, bool bRecursive);

void loop_input(bool bPrintReverse);

void print_node(node *npStart, char *currentWord, bool bPrintReverse);

char *concat_char_to_word(char *cpStartString, char c);

int char_to_idx(char c, int iArrLen);

#endif //SYSTEMS_EX04_FREQUENCY_H
