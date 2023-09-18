#ifndef ENCODE_H
#define ENCODE_H

#include <stdlib.h>
#include "lexical_analyzer.h"
#include "data_structure.h"
#include "consts.h"



/*
    this function encodes an instruction line into the code image.
    @param line The line to encode
    @param lt The lex tree of the line (Abstract Syntax Tree)
    @param code_image The code image to encode the line into
    @param ic The instruction counter
    @param symbol_table The symbol table.
    @return TRUE if the line was encoded successfully, FALSE otherwise
*/
bool encode_instruction(line_info *line, struct lex_tree *tree, machine_word *code_image, long *ic, symbol_table *symbol_table);


/*
    Encodes a first word of a command.
    @param command_code The command code.
    @param first_operand_type The first operand type.
    @param second_operand_type The second operand type.
    @param ic The instruction counter.
    @param code_image The code image.
    @param param1_type The first parameter type.
    @param param2_type The second parameter type.
*/
void encode_first_word(enum inst_name command_code, enum operand_addr_type first_operand_type, enum operand_addr_type second_operand_type,
                              long *ic, machine_word code_image[]);

/*
    Encodees an immediate addressing type word.
    @param operand The operand to be encoded.
    @param ic The instruction counter.
    @param code_image The code image.
*/
void encode_immediate_word(int operand, long *ic, machine_word code_image[]);

/*
    Encodes a label addressing type word.
    @param operand The operand to be encoded.
    @param ic The instruction counter.
    @param code_image The code image.
*/
void encode_label_word(char label_operand[], long *ic, machine_word code_image[]);

/*
    Encodes a register addressing type word.
    @param first_register The first register to be encoded.
    @param second_register The second register to be encoded.
    @param ic The instruction counter.
    @param code_image The code image.
*/
void encode_register_word(char first_register, char second_register, long *ic, machine_word code_image[]);

/*
    Encodes an integer to a 10 bits word.
    @param value The integer to be encoded.
    @return The encoded word.
*/
imdt_drct_word encode_int_10_bits(int value);


/*
    This functions gets a char and encodes it to a data word.
    @param c The char to be encoded.
    @return The encoded data word.
*/
data_word encode_char(char c);


/*
    this function gets a string and encodes it to the data image.
    @param str The string to be encoded.
    @param data_image The data image to be encoded to.
    @param dc The data counter.
*/
void encode_string(char str[], data_word *data_image, long *dc);


/*
    this function gets an array of integers and encodes it to the data image.
    @param num_array The array of integers to be encoded.
    @param num_count The number of integers in the array.
    @param data_image The data image to be encoded to.
    @param dc The data counter.
*/
void encode_data(int num_array[], int num_count, data_word *data_image, long *dc);


/*
    This functions gets an integer and encodes it to a data word.
    @param value The integer to be encoded.
    @return The encoded data word.
*/
data_word encode_data_int_10_bits(short int value);


#endif
