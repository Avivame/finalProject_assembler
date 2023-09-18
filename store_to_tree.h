#ifndef SRORE_TO_TREE_H
#define SRORE_TO_TREE_H
#include "lexical_analyzer.h"


#define MAX_INT_VALUE 8191
#define MIN_INT_VALUE -8192

/*
    This function gets a line and stores the directive (.string/ .entry/ .extern) in the lex tree.
    @param line - the line to parse
    @param index - the index of the line to start parsing from
    @param tree - the lexer tree to store the directive in
*/
void store_directive(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the string in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the string in
*/
void store_string(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the data in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the data in
*/
void store_data(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the entry/extern label in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the entry/extern label in
*/
void store_ent_extern_label(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the instruction in
*/
void store_code_instruction(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the operands of a set A instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the operands in
*/
void store_set_a_operands(const char *line, int *index, struct lex_tree *tree);

/*
    This function gets a line and stores the operands of a set B instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param tree - the lexer tree to store the operands in
*/
void store_set_b_operands(const char *line, int *index, struct lex_tree *tree);

#endif
