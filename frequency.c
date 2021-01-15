//
// Created by Yuri Grigorian on 15/01/2021.
//

#include <ctype.h>
#include <string.h>
#include "frequency.h"
#include "stdio.h"

/// Allocated a new node.
/// \param cLetter Char for initial setup.
/// \param iCount Initial count.
/// \return Allocated node ptr.
node *allocate_new_node(char cLetter, int iCount) {
    node *n = (node *) malloc(sizeof(node));
    if (n == NULL) { return n; }

    n->letter = cLetter;
    n->count = iCount;

    for (int i = 0; i < NUM_LETTERS; ++i) {
        n->children[i] = NULL;
    }
    return n;
}

/// Releases node resources. Optionally free children.
/// \param npNode node ptr to free.
/// \param bRecursive Should free children.
/// \return Released node count.
int release_node(node *npNode, bool bRecursive) {
    int iReleased = 1;

    if (bRecursive) {
        for (int i = 0; i < NUM_LETTERS; ++i) {
            node *npLetterChildNode = npNode->children[i];
            if (npLetterChildNode != NULL) {
                iReleased += release_node(npLetterChildNode, bRecursive);
            }
        }
    }

    free(npNode);
    return iReleased;
}

/// Returns lower case char from stdin.
/// \return
char read_char_stdin_lower() {
    char c = (char) getchar();
    // Thankfully chars are unsigned here.
    return c == EOF ? EOF : tolower(c);
}

/// Converts a char to index boundaries.
/// \param c char to add
/// \param iArrLen Arr size.
/// \return Index in array.
int char_to_idx(char c, int iArrLen) {
    return c % iArrLen;
}

/// Concats a null terminated string with a char. Return NULL on error. Returns a new str ptr.
/// \param cpStartString string
/// \param c char to add.
/// \return New string ptr.
char *concat_char_to_word(char *cpStartString, char c) {
    if (c == CHAR_NULL) {
        return NULL;
    }

    // cpStartString len + char + NULL terminator
    unsigned long iAllocSize = (cpStartString != NULL ? strlen(cpStartString) : 0) + 1 + 1;
    char *cpNewString = (char *) malloc(iAllocSize);
    int iFormatStatus = 0;

    if (cpNewString == NULL) { return NULL; }

    if (cpStartString != NULL) {
        iFormatStatus = sprintf(cpNewString, "%s%c", cpStartString, c);
    } else {
        iFormatStatus = sprintf(cpNewString, "%c", c);
    }

    if (iFormatStatus < 0) { return NULL; }

    return cpNewString;
}

/// Prints words in trie.
/// \param npStart Starting Node.
/// \param currentWord Current concated string.
/// \param bPrintReverse Should prioritize Z over A.
void print_node(node *npStart, char *currentWord, bool bPrintReverse) {
    if (npStart == NULL) { return; }

    char *newWord = concat_char_to_word(currentWord, npStart->letter);

    // If we got a a word ending, print it.
    if (npStart->count > 0) {
        printf("%s %lu\n", newWord, npStart->count);
    }

    int startingLetterIdx = bPrintReverse ? char_to_idx(CHAR_Z_LOWER, NUM_LETTERS) :
                            char_to_idx(CHAR_A_LOWER, NUM_LETTERS);

    for (int i = 0; i < NUM_LETTERS; ++i) {
        int letterIdx = bPrintReverse ? (startingLetterIdx - i) % NUM_LETTERS :
                        (startingLetterIdx + i) % NUM_LETTERS;

        // Handle negative indexes
        letterIdx = letterIdx < 0 ? NUM_LETTERS + letterIdx : letterIdx;

        // Recurse to child
        node *childLetter = npStart->children[letterIdx];
        print_node(childLetter, newWord, bPrintReverse);
    }

    free(newWord);
}

/// Reads input from user and build the trie. Ignore all chars except a-z. Treats whitespace, EOF and \n as word ending.
/// \param bPrintReverse Should print in reverse.
void loop_input(bool bPrintReverse) {
    node *npRoot = allocate_new_node(CHAR_NULL, 0);
    node *currentNode = npRoot;
    char c;

    while ((c = read_char_stdin_lower())) {
        // NOTE: Assigment told to ignore non alpha chars but provided test inputs that included two lines.
        // Gonna treat \n as whitespace.

        // If at ending of word
        if ((c == CHAR_SEP || c == EOF || c == CHAR_NEWLINE) && currentNode != npRoot) {
            currentNode->count += 1;
            currentNode = npRoot;

        } else if (c >= CHAR_A_LOWER && c <= CHAR_Z_LOWER) {
            node *childLetter = currentNode->children[char_to_idx(c, NUM_LETTERS)];

            // Allocate a letter if its first seen.
            if (childLetter == NULL) {

                // Assuming success here.
                childLetter = allocate_new_node(c, 0);
                currentNode->children[char_to_idx(c, NUM_LETTERS)] = childLetter;
            }
            // Set current pointer to child letter.
            currentNode = childLetter;
        }

        // Time to exit.
        if (c == EOF) { break; }

    }

    print_node(npRoot, NULL, bPrintReverse);
    release_node(npRoot, true);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], ARG_REVERSE) == 0) {
        loop_input(true);
    } else {
        loop_input(false);
    }
}