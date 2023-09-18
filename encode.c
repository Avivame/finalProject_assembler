#include <stdlib.h>
#include "utils.h"
#include "lexical_analyzer.h"
#include "data_structure.h"
#include "consts.h"
#include "encode.h"


imdt_drct_word encode_int_10_bits(int value) {
    imdt_drct_word result;
    /* Convert negative value to two's complement representation */
    if (value < 0) {
        unsigned int abs_value = abs(value);

        /* Complement and add one to get two's complement representation */
        unsigned int complement = (~abs_value) + 1;

        /* Set the most significant bit to indicate negative number */
        result.operand = (1 << 9) | (complement & 0x1FFF);
    } else {
        /* Set positive value to binary representation */
        result.operand = value & 0x1FFF;
    }
    return result;
}


void encode_first_word(enum inst_name command_code, enum operand_addr_type first_operand_type, enum operand_addr_type second_operand_type,
                              long *ic, machine_word code_image[]){
    machine_word word;
    word.label = NULL;
    word.word.f_word = (first_word*)calloc(1, sizeof(first_word));
    word.word.f_word->op_code = command_code;
    word.word.f_word->source_op_addr = first_operand_type;
    word.word.f_word->target_op_addr = second_operand_type;
    code_image[(*ic)++] = word;
}


void encode_immediate_word(int operand, long *ic, machine_word code_image[]){
    machine_word word = {0};
    word.label = NULL;
    word.word.id_word = (imdt_drct_word*)calloc(1, sizeof(imdt_drct_word));
    *(word.word.id_word) = encode_int_10_bits(operand);
    word.word.id_word->ARE = 0;
    code_image[(*ic)++] = word;
}


void encode_label_word(char label_operand[], long *ic, machine_word code_image[]){
    machine_word word;
    int len;
    for(len=0; label_operand[len] != '\0'; len++); /* Get the length of the label */
    word.label = calloc(len+1, sizeof(char));
    word.label[len] = '\0';
    word.word.f_word = NULL;
    strcpy(word.label, label_operand);
    code_image[(*ic)++] = word;
}


void encode_register_word(char first_register, char second_register, long *ic, machine_word code_image[]){
    machine_word word;
    word.label = NULL;
    word.word.r_word = (register_word*)calloc(1,sizeof(register_word));
    if(first_register >= 0){
        word.word.r_word->source_register = first_register - '0';
    }
    if(second_register >= 0){
        word.word.r_word->target_register = second_register - '0';
    }
    code_image[(*ic)++] = word;
}


bool encode_instruction(line_info *line, struct lex_tree *tree, machine_word *code_image, long *ic, symbol_table *symbol_table){
    /* Encoding first set commands - mov, cmp, add, sub, lea (have source operand)*/
    if(tree->line_inst_line_dir.inst_line.inst_name == mov_inst || tree->line_inst_line_dir.inst_line.inst_name == cmp_inst ||
            tree->line_inst_line_dir.inst_line.inst_name == add_inst || tree->line_inst_line_dir.inst_line.inst_name == sub_inst ||
            tree->line_inst_line_dir.inst_line.inst_name == lea_inst){
        /* lea just have an operand of the type label*/
        if(tree->line_inst_line_dir.inst_line.inst_name == lea_inst){
            if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_immed ||
               tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_register){
                print_error_message(line, "The source operand for lea command can't be of label address type");
                return FALSE;
            }
            else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_immed){
                print_error_message(line, "The target operand for lea command can't be of immediate address type");
                return FALSE;
            }
        }
        /* mov, cmp, add, sub */
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_immed &&
           tree->line_inst_line_dir.inst_line.inst_name != cmp_inst){
            print_error_message(line, "The target operand for mov, add and sub command can't be of immediate address type");
            return FALSE;
        }
        encode_first_word(tree->line_inst_line_dir.inst_line.inst_name, tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0],
                          tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1], ic, code_image);
        /* Both operands are registers */
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_register &&
           tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_register){
            encode_register_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].reg_val,
                                 tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].reg_val,
                                 ic, code_image);
            return TRUE;
        }
        /* First operand immediate*/
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_immed){
            encode_immediate_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].immed_val,
                                  ic, code_image);
        }
            /* First operand label */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_label){
            encode_label_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].label_val,
                              ic, code_image);
        }
            /* First operand register */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_register){
            encode_register_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].reg_val,
                                 -1, ic, code_image);
        }
        /* Second operand immediate */
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_immed){
            encode_immediate_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].immed_val,
                                  ic, code_image);
            return TRUE;
        }
            /* Second operand label */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_label){
            encode_label_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].label_val,
                              ic, code_image);
            return TRUE;
        }
            /* Second operand register */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_register){
            encode_register_word(-1, tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].reg_val,
                                 ic, code_image);
            return TRUE;
        }
    }
    /* Encoding second set commands - not, clr, inc, dec, red, jmp, bne, prn, jsr*/
    if(tree->line_inst_line_dir.inst_line.inst_name == not_inst || tree->line_inst_line_dir.inst_line.inst_name == clr_inst ||
       tree->line_inst_line_dir.inst_line.inst_name == inc_inst || tree->line_inst_line_dir.inst_line.inst_name == dec_inst ||
       tree->line_inst_line_dir.inst_line.inst_name == red_inst || tree->line_inst_line_dir.inst_line.inst_name == jmp_inst ||
       tree->line_inst_line_dir.inst_line.inst_name == bne_inst || tree->line_inst_line_dir.inst_line.inst_name == prn_inst ||
       tree->line_inst_line_dir.inst_line.inst_name == jsr_inst)
    {
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_immed &&
        tree->line_inst_line_dir.inst_line.inst_name != prn_inst){
            tree->lex_line_option = lex_line_error;
            print_error_message(line, "Only prn command have an immediate operand addressing in set b");
            return FALSE;
            }
        encode_first_word(tree->line_inst_line_dir.inst_line.inst_name, 0 ,
                          tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat, ic, code_image);

        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_immed){
            encode_immediate_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.immed_val,
                                  ic, code_image);
            return TRUE;
        }
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_label){
            encode_label_word(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.label_val,
                              ic, code_image);
            return TRUE;
        }
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_register){
            encode_register_word(-1, tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.reg_val,
                                 ic, code_image);
            return TRUE;
        }
    }

    /* Encoding third set commands - rts, stop */
    if(tree->line_inst_line_dir.inst_line.inst_name == rts_inst || tree->line_inst_line_dir.inst_line.inst_name == stop_inst){
        encode_first_word(tree->line_inst_line_dir.inst_line.inst_name, 0, 0, ic, code_image);
        return TRUE;
    }
    return TRUE;
}


data_word encode_char(char c){
    data_word word = {0};
    unsigned int n = (unsigned int)c;
    word.data = n;
    return word;
}

void encode_string(char str[], data_word *data_image, long *dc){
    int i;
    for(i = 0; str[i] != '\0'; i ++){
        data_image[(*dc)++] = encode_char(str[i]);
    }
    data_image[(*dc)++] = encode_char('\0');
}


data_word encode_data_int_10_bits(short int value) {
    data_word result;

    if (value < 0) {
        /* Convert negative value to two's complement representation */

        /* Convert to absolute value */
        unsigned int abs_value = abs(value);

        /* Complement and add one to get two's complement representation */
        unsigned int complement = (~abs_value) + 1;

        /* Set the most significant bit to indicate negative number */
        result.data = (1 << 9) | (complement & 0x1FFF);
    } else {
        /* Set positive value to binary representation */
        result.data = value & 0x1FFF;
    }

    return result;
}


void encode_data(int num_array[], int num_count, data_word *data_image, long *dc){

    int i;
    for(i = 0; i < num_count; i ++){
        data_image[(*dc)++] = encode_data_int_10_bits(num_array[i]);
    }
}
