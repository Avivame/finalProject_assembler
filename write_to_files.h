#ifndef FINAL_PROJECT_C_WRITE_TO_FILES_H
#define FINAL_PROJECT_C_WRITE_TO_FILES_H
#include <stdio.h>
#include <stdlib.h>
#include "consts.h"
#include "utils.h"
#include "data_structure.h"


/*
    This function generate the base64 representation
    @param value: an unsigned integer value (12 bits) that you want to convert to base64.
    @return a dynamically allocated string containing the base64 representation of the input value.
*/
char *convert_to_base64(unsigned int value);

/*
    This function converts the signed value to a 12-bit two's complement representation (if it's negative).
    This function handles both positive and negative values.
    @param value: a signed integer value (12 bits) that you want to convert to base64.
    @return a dynamically allocated string containing the base64 representation of the input value

*/
char *convert_signed_to_base64(int value);

/*
    This function writes the object file.
    @param filename: The name of the file to write to
    @param code_image The code image to write
    @param ic The instruction counter
    @param data_image The data image to write
    @param dc The data counter
*/
void write_obj_file(char *filename, machine_word code_image[], long ic, data_word data_image[], long dc);

/*
    This function writes the entry file.
    @param filename The name of the file to write to
    @param symbol_table The symbol table
*/
void write_ent_file(char *filename, symbol_table *symbol_table);

/*
    This function writes the external file.
    @param filename The name of the file to write to
    @param symbol_table The symbol table
    @param code_image The code image
    @param ic The instruction counter
*/
void write_ext_file(char *filename, symbol_table *symbol_table, machine_word code_image[], long ic);

#endif
