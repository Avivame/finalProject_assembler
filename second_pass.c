#include "second_pass.h"
#include "data_structure.h"
#include "consts.h"
#include "lexical_analyzer.h"
#include "utils.h"


bool second_pass(struct lex_tree tree, symbol_table *table, line_info line){
    symbol_table_entry *entry;
    /* If it is instruction verify that all the labels used in the instruction are defined! */
    if(tree.lex_line_option == lex_line_inst){
        if(tree.line_inst_line_dir.inst_line.inst_name == add_inst || tree.line_inst_line_dir.inst_line.inst_name == sub_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == cmp_inst || tree.line_inst_line_dir.inst_line.inst_name == lea_inst ||
           tree.line_inst_line_dir.inst_line.inst_name == mov_inst){
            if(tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_label){

                if(get_from_symbol_table(table, tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].label_val) == NULL){
                    print_error_message(&line, "The label %s is used but not defined", tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].label_val);
                    return FALSE;
                }
            }
            else if(tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_label){
                if(get_from_symbol_table(table, tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].label_val) == NULL){
                    print_error_message(&line, "The label %s is used but not defined", tree.line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].label_val);
                    return FALSE;
                }
            }
        }
        else if(tree.line_inst_line_dir.inst_line.inst_name == inc_inst || tree.line_inst_line_dir.inst_line.inst_name == dec_inst ||
                tree.line_inst_line_dir.inst_line.inst_name == not_inst || tree.line_inst_line_dir.inst_line.inst_name == clr_inst ||
                tree.line_inst_line_dir.inst_line.inst_name == prn_inst || tree.line_inst_line_dir.inst_line.inst_name == red_inst ||
                tree.line_inst_line_dir.inst_line.inst_name == jsr_inst || tree.line_inst_line_dir.inst_line.inst_name == bne_inst ||
                tree.line_inst_line_dir.inst_line.inst_name == jmp_inst) {
            if (tree.line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_label) {
                if (get_from_symbol_table(table,tree.line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.label_val) == NULL) {
                    print_error_message(&line, "The label %s is used but not defined", tree.line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.label_val);
                    return FALSE;
                }
            }
        }
        return TRUE;
    }
    if(tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_entry){
        entry = get_from_symbol_table(table, tree.line_inst_line_dir.dir_line.dir_var.label);
        if(entry == NULL){
            print_error_message(&line, "The label %s is used but not defined", tree.line_inst_line_dir.dir_line.dir_var.label);
            return FALSE;
        }
        else{
            entry->symbol_type = ENTRY_SYMBOL;
        }
    }
    else if(tree.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_extern){
        entry = get_from_symbol_table(table, tree.line_inst_line_dir.dir_line.dir_var.label);
        if(entry == NULL){
            print_error_message(&line, "Extern label is declared but not used in the file");
            return FALSE;
        }
        else{
            entry->symbol_type = EXTERN_SYMBOL;
            return TRUE;
        }
    }
    return TRUE;
}


void label_encoding(symbol_table *symbol_table, machine_word *code_image, long ic){
    int i;
    for(i=0; i < ic; i++){
        if(code_image[i].label != NULL){
            symbol_table_entry *entry = get_from_symbol_table(symbol_table, code_image[i].label);
            if(entry == NULL){
                continue;
            }
            if(entry->symbol_type == EXTERN_SYMBOL){
                code_image[i].word.id_word = (imdt_drct_word*)calloc(1,sizeof(imdt_drct_word));
                code_image[i].word.id_word->ARE = 1;
                code_image[i].word.id_word->operand = 0;
            }
            else{
                code_image[i].word.id_word = (imdt_drct_word*)calloc(1, sizeof(imdt_drct_word));
                code_image[i].word.id_word->ARE = 2;
                code_image[i].word.id_word->operand= (entry->symbol_addr) + IC_INIT_VALUE;
            }
        }
    }
}
