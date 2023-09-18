#include "consts.h"
#include "utils.h"
#include "data_structure.h"
#include "encode.h"
#include "lexical_analyzer.h"




bool first_pass(line_info line, symbol_table *symbol_table, long *ic, long *dc,
                             data_word *data_image, machine_word *code_image, struct lex_tree *tree) {
    /* Line is a label definition */
    if(tree->label[0] != '\0'){
        /* check if label already defined */
        if(get_from_symbol_table(symbol_table, tree->label) != NULL){
            if(get_from_symbol_table(symbol_table, tree->label)->symbol_type == EXTERN_SYMBOL){
                print_error_message(&line, "Extern label can't defined more then one time");
                return FALSE;
            }
            else{
                print_error_message(&line, "Extern label already defined.");
                return FALSE;
            }
        }
        if(tree->lex_line_option == lex_line_dir){
            if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_string ||
               tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_data){
                if(tree->label &&  get_from_symbol_table(symbol_table, tree->label) == NULL){
                    add_entry(symbol_table, tree->label, *dc, DATA_SYMBOL);
                }
                if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_string){
                    encode_string(tree->line_inst_line_dir.dir_line.dir_var.string, data_image, dc);
                }
                else{
                    encode_data(tree->line_inst_line_dir.dir_line.dir_var.num_array.num_array,
                                tree->line_inst_line_dir.dir_line.dir_var.num_array.num_count, data_image, dc);

                }
            }
            else if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_entry ||
                    tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_extern){
                print_error_message(&line, ".entry or .extern in label definition are useless.");
                return FALSE;
            }
        }
        if(tree->lex_line_option == lex_line_inst){
            add_entry(symbol_table, tree->label, *ic, CODE_SYMBOL);
            return encode_instruction(&line, tree, code_image, ic, symbol_table);
        }

    }
    else{/* Not a label decleration */
        if(tree->lex_line_option == lex_line_dir){
            if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_string ||
               tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_data){
                if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_string){
                    encode_string(tree->line_inst_line_dir.dir_line.dir_var.string, data_image, dc);
                }
                else{
                    encode_data(tree->line_inst_line_dir.dir_line.dir_var.num_array.num_array,
                                tree->line_inst_line_dir.dir_line.dir_var.num_array.num_count, data_image, dc);

                }
            }
            else if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_extern){
                add_entry(symbol_table, tree->line_inst_line_dir.dir_line.dir_var.label, 0, EXTERN_SYMBOL);
                return TRUE;
            }
            else if(tree->line_inst_line_dir.dir_line.lex_dir_option == lex_dir_entry){
                return TRUE;
            }
        }
        else if(tree->lex_line_option == lex_line_inst){
            return encode_instruction(&line, tree, code_image, ic, symbol_table);
    }
    }
    return TRUE;
}
