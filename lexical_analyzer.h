#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#define MAX_LABEL_SIZE 31
#define MAX_LEX_ERROR_LEN 150
#define MAX_STRING_LEN 80
#define MAX_NUMBER_SIZE 80

enum inst_name {
    /* Set A commands */
    mov_inst,
    cmp_inst,
    add_inst,
    sub_inst,
    lea_inst,
    /* Set B commands */
    not_inst,
    clr_inst,
    inc_inst,
    dec_inst,
    jmp_inst,
    bne_inst,
    red_inst,
    prn_inst,
    jsr_inst,
    /* Set C commands */
    rts_inst,
    stop_inst
};

/* operand address type */
enum operand_addr_type {
    lex_op_addr_type_error = -1,
    lex_op_addr_type_immed = 1,
    lex_op_addr_type_label = 3,
    lex_op_addr_type_register = 5
};

/* operand values */
union operand_addr_val {
    char label_val[MAX_LABEL_SIZE +1];
    char reg_val;
    int immed_val;
};

/* structure of Abstract Syntax Tree for each line */
struct lex_tree {
    char label[MAX_LABEL_SIZE + 1];
    char lex_error[MAX_LEX_ERROR_LEN + 1];
    enum {
        lex_line_error = -1,
        lex_line_inst,   /* commands */
        lex_line_dir   /* .string, .extern, .entry */
    }lex_line_option;
    /* check if line is a instruction, directive or error line and insert the necessary information to the tree */
    union {
        struct {
            enum {
                lex_dir_string,
                lex_dir_data,
                lex_dir_entry,
                lex_dir_extern
            }lex_dir_option;
            union {
                char string[MAX_STRING_LEN +1];
                struct {
                    int num_array[MAX_NUMBER_SIZE];
                    int num_count;
                }num_array;
                char label[MAX_LABEL_SIZE +1];
            }dir_var;
        }dir_line;
        struct {
            enum inst_name inst_name;
            union {
                struct {
                    enum operand_addr_type oat[2];
                    union operand_addr_val oatv[2];
                }inst_set_a;
                struct {
                    enum operand_addr_type oat;
                    union operand_addr_val oatv;
                }inst_set_b;
            }inst_sets;
        }inst_line;
    }line_inst_line_dir;
};

/*
    This function gets a line and returns an AST of the line.
    @param line - the line to parse
    @return The Abstract Syntax Tree of the line (struct lex_tree)
*/
struct lex_tree get_lex_tree_from_line(const char * line);

#endif
