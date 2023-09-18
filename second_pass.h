#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "data_structure.h"
#include "consts.h"
#include "lexical_analyzer.h"

/*
    This function processes a line in the second pass of the assembler.
    @param tree The lex tree of the line (Abstract Syntax Tree)
    @param table The symbol table
    @param line The processed line information
    @return TRUE if the line was processed successfully, FALSE otherwise
*/
bool second_pass(struct lex_tree tree, symbol_table *table, line_info line);

/*
    This function encodes the labels into the code image.
    @param symbol_table The symbol table
    @param code_image The code image
    @param ic The instruction counter
*/
void label_encoding(symbol_table *symbol_table, machine_word *code_image, long ic);

#endif
