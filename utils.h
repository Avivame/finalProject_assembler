#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "consts.h"
#include "data_structure.h"
#include "lexical_analyzer.h"

/*
 * moves the index to the next place in string where the char isn't white
*/
#define MOVE_NEXT_TXT(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)));


/*
    checks if a string is all digits.
    @param operand The string to check.
    @return TRUE if the string is all digits.
*/
bool is_number_operand(const char *operand);


/*
    Checks if a string is a valid label.
    @param word The string to check.
    @return TRUE if the string is a valid label.
*/
bool is_label_operand(char *word);


/*
    This function check each operand (of instruction line) which type of address it is.
    @param operand - operand of an instruction line
    @param tree - the lexer tree to store the operand address in
*/
enum operand_addr_type analyze_operand(char *operand, struct lex_tree *tree);


/*
    checks if a string is all digits.
    @param operand The string to check.
    @return TRUE if the string is all digits.
*/
char *str_allocate_cat(char *first_str, char* second_str);


/*
    Prints an error message to the standard output.
    @param line The line where the error occured (contains info about the file name and content of the line)
    @param format The formatted message to print.
    @param ... The variables needed to format the message. (Like in printf).
*/
void print_error_message(line_info *line, const char *format, ...) ;


/*
    Prints a data word to the standard output.
    @param word The data word to print.
*/

void print_data_word(data_word word);


/*
    Prints a machine word to the standard output.
    @param word The machine word to print.
*/
void print_machine_word(machine_word word);


/*
    Freeing the code image.
    @param code_image The code image to be free
    @param ic The number of values in the list.
*/
void free_code_image(machine_word code_image[], long ic);


/*
    Updates the data symbols address in the symbol table.
    @param table The symbol table to update.
    @param ic The number of words in the code image.
*/
void update_data_symbols_address(symbol_table *table, long ic);

#endif
