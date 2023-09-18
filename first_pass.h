#include "consts.h"
#include "data_structure.h"
#include "lexical_analyzer.h"


/*
    Processes a line in the first pass of the assembler.
    @param line - the line to process
    @param symbol_table - The symbol table
    @param ic The instruction counter
    @param dc The data counter
    @param data_image The data image
    @param code_image The code image
    @param lt The lex tree of the line (Abstract Syntax Tree)
    @return TRUE if the line was processed successfully, FALSE otherwise
*/
bool first_pass(line_info line, symbol_table *symbol_table, long *ic, long *dc,
                             data_word *data_image, machine_word *code_image, struct lex_tree *tree);
