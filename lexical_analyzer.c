#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "lexical_analyzer.h"
#include "utils.h"
#include "store_to_tree.h"


struct lex_tree get_lex_tree_from_line(const char * line) {
    struct lex_tree tree = {0};
    char *ptr;
    int i = 0, index = 0;
    char *token = NULL;

    MOVE_NEXT_TXT(line, i);
    ptr = (char *)(line +i);

    /* Check if the line is a label definition */
    MOVE_NEXT_TXT(line, i)
    if((ptr = strchr(line, ':')) != NULL){
        index = ptr - (line + i);
        token = malloc(sizeof(char) * index + 1);
        strncpy(token, line + i, index);
        token[index] = '\0';
        /* if label - update in tree that it is a label */
        if(is_label_operand(token)){
            strcpy(tree.label, token);
            free(token);
            index += (i + 1); /* Skip the ':' */
        }
        else{
            tree.lex_line_option = lex_line_error;
            strcpy(tree.lex_error, "Invalid label");
            free(token);
            return tree;
        }
    }

    else{
        tree.label[0] = '\0'; /* update in tree that it is not a label */
    }

    /* Check if the line is a directive (.string .data .entry .extern) */
    MOVE_NEXT_TXT(line, index)
    if(line[index] == '.'){
        store_directive(line, &index, &tree);
        MOVE_NEXT_TXT(line, index)
        if(tree.lex_line_option == lex_line_error){
            return tree;
        }
        else if(tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_string){
            store_string(line, &index, &tree);
            return tree;
        }
        else if(tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_data){
            store_data(line, &index, &tree);
        }
        else if(tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_entry ||
                tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_extern){
            store_ent_extern_label(line, &index, &tree);
            return tree;
        }
    }

    /* the line is a instruction */
    else {
        store_code_instruction(line, &index, &tree);
        if(tree.lex_line_option == lex_line_error){
            return tree;
        }
        MOVE_NEXT_TXT(line, index)
        /* First set commands (2 Parameters) - mov, cmp, add, sub, lea */
        if(tree.line_inst_line_dir.inst_line.inst_name == mov_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == cmp_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == add_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == sub_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == lea_inst)
        {
            store_set_a_operands(line, &index, &tree);
            return tree;
        }

        /* Second set commands (1 Parameter) - not, clr, inc, dec, jmp, bne, red, prn, jsr */
        if(tree.line_inst_line_dir.inst_line.inst_name == not_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == clr_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == inc_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == dec_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == jmp_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == bne_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == red_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == prn_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == jsr_inst)
        {
            store_set_b_operands(line, &index, &tree);
            return tree;
        }

        /* Third set commands (No parameters) - stop, rts */
        if(tree.line_inst_line_dir.inst_line.inst_name == stop_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == rts_inst){
            MOVE_NEXT_TXT(line, index)
            if(line[index] != '\0' && line[index] != '\n' && line[index] != EOF){
                tree.lex_line_option = lex_line_error;
                strcpy(tree.lex_error, "Third instructions group do not have operands.");
                return tree;
            }
            return tree;
        }
    }

    return tree;
}
