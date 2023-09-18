
#ifndef UNTITLED5_START_H
#define UNTITLED5_START_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORDS 80
#define MAX_WORD_LENGTH 50

#define MAX_LINE_LENGTH 80
#define MAX_MACRO_NAME_LENGTH 50

#define BEGGING_OF_MACRO "mcro"
#define END_OF_MACRO "endmcro"

#define FixLine \
if (line[strlen(line) - 1] == '\n') { \
    line[strlen(line) - 1] = '\0'; \
}

void getArrayFromLine(const char *sentence) ;
char* get_next_line(FILE* file) ;
char words[MAX_WORDS][MAX_WORD_LENGTH];
int preAssembler(const char* filename) ;
int is_macro_name_of_func();


/**
 * Opens the specified file for reading.
 *
 * @param fileName The name of the file to be opened.
 * @return A pointer to the opened file.
 */
FILE* openFileForReading(const char *fileName);
/**
 * Opens a file for writing and returns a FILE pointer to the opened file.
 *
 * @param fileName The name of the file to be opened for writing.
 * @return A FILE pointer to the opened file for writing.
 */
FILE* openFileForWriting(const char *fileName);
#endif 
