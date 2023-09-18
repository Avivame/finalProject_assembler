#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "lexical_analyzer.h"
#include "store_to_tree.h"
#include "utils.h"


void store_directive(const char *line, int *index, struct lex_tree *tree){
    (*index)++; /* Skip the '.' */
    if(strncmp(line+ (*index) , "string", 6) == 0){
        tree->lex_line_option = lex_line_dir;
        tree->line_inst_line_dir.dir_line.lex_dir_option = lex_dir_string;
        (*index) += 6; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "data", 4) == 0){
        tree->lex_line_option = lex_line_dir;
        tree->line_inst_line_dir.dir_line.lex_dir_option = lex_dir_data;
        (*index) += 4; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "entry", 5) == 0){
        tree->lex_line_option = lex_line_dir;
        tree->line_inst_line_dir.dir_line.lex_dir_option = lex_dir_entry;
        (*index) += 5; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "extern", 6) == 0){
        tree->lex_line_option = lex_line_dir;
        tree->line_inst_line_dir.dir_line.lex_dir_option = lex_dir_extern;
        (*index) += 6; /* Skip the directive */
    }
    else{
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Invalid instruction.");
        return;
    }
}


void store_string(const char *line, int *index, struct lex_tree *tree){
    char *end_quote;
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, ".string instruction must have a string.");
        return;
    }
    if(line[*index] != '"'){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "String must start with quotes(\").");
        return;
    }
    (*index)++; /* Skip the '"' */

    /* Check for quotes ends */
    end_quote = strchr(line + (*index), '"');
    if(end_quote == NULL){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "String must end with quotes(\").");
        return;
    }
    else{
        int len = end_quote - (line + (*index));
        strncpy(tree->line_inst_line_dir.dir_line.dir_var.string, line + (*index), len);
        tree->line_inst_line_dir.dir_line.dir_var.string[len] = '\0';
        (*index) += len + 1; /* Skip the '"' */
    }
    MOVE_NEXT_TXT(line, (*index))
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "should not be characters after the end of the string");
        return;
    }
}

void store_data(const char *line, int *index, struct lex_tree *tree){
    char *endPtr;
    int value;
    int num_of_ints = 0;

    do{
        MOVE_NEXT_TXT(line, (*index))
        if(line[*index] == ','){
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "must start with an integer.");
            return;
        }
        else if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, ".data instruction must have at least one integer.");
            return;
        }
        /* get the data string digit as a number (in 10 base)*/
        value = strtol(line + (*index), &endPtr, 10);
        if(endPtr == line + (*index)){
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "Invalid number.");
            return;
        }
        if(value > MAX_INT_VALUE || value < MIN_INT_VALUE){
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "Number out of range.");
            return;
        }
        tree->line_inst_line_dir.dir_line.dir_var.num_array.num_array[num_of_ints] = value;
        tree->line_inst_line_dir.dir_line.dir_var.num_array.num_count++;
        num_of_ints++;
        (*index) += endPtr - (line + (*index));
        MOVE_NEXT_TXT(line, (*index))
        if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
            return;
        }
        else{
            if(line[*index] != ','){
                tree->lex_line_option = lex_line_error;
                strcpy(tree->lex_error, "Invalid character, expected ',' sign.");
                return;
            }
            (*index)++; /* Skip the ',' */
            MOVE_NEXT_TXT(line, (*index))
            if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
                tree->lex_line_option = lex_line_error;
                strcpy(tree->lex_error, "There is no number after ',' sign.");
                return;
            }
            else if(line[*index] == ','){
                tree->lex_line_option = lex_line_error;
                strcpy(tree->lex_error, "Should be one comma after each data number.");
                return;
            }
        }

    }
    while(*endPtr != '\n' && *endPtr != '\0' && *endPtr != EOF);

}

void store_ent_extern_label(const char *line, int *index, struct lex_tree *tree){
    char *token = NULL;
    int len = 0;
    MOVE_NEXT_TXT(line, (*index))
    if(tree->label[0] != '\0'){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Entry/Extern instruction can't be in label definition.");
        return;
    }
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Should have label after entry/extern directive definition.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    token = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(token, line + (*index), len);
    token[len] = '\0';
    (*index) += len;


    if(!is_label_operand(token)){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Invalid label.");
        free(token);
        return;
    }
    strcpy(tree->line_inst_line_dir.dir_line.dir_var.label, token);
    free(token);
    MOVE_NEXT_TXT(line, (*index))
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "should not be characters after the end of the label.");
        return;
    }
}


void store_code_instruction(const char *line, int *index, struct lex_tree *tree){
    int len;
    char *optional_command = NULL;
    struct command{char *name; enum inst_name inst;};
    struct command commands_list[] = {
            {"mov", mov_inst},
            {"cmp", cmp_inst},
            {"add", add_inst},
            {"sub", sub_inst},
            {"not", not_inst},
            {"clr", clr_inst},
            {"lea", lea_inst},
            {"inc", inc_inst},
            {"dec", dec_inst},
            {"jmp", jmp_inst},
            {"bne", bne_inst},
            {"red", red_inst},
            {"prn", prn_inst},
            {"jsr", jsr_inst},
            {"rts", rts_inst},
            {"stop", stop_inst}
    };
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    optional_command = malloc(sizeof(char) * len + 1);
    strncpy(optional_command, line + *index, len);
    optional_command[len] = '\0';
    (*index) += len;

    tree->lex_line_option = lex_line_inst;
    {
        struct command *ptr = NULL;
        int i;
        for(ptr = commands_list, i = 0; i < 16; i++, ptr++){
            if(strcmp(optional_command, ptr->name) == 0){
                tree->line_inst_line_dir.inst_line.inst_name = ptr->inst;
                return;
            }
        }
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Command doesn't exist.");
    }
}


void store_set_a_operands(const char *line, int *index, struct lex_tree *tree){
    char *comma;
    int len;
    char *operand;
    if((comma = strchr(line + *index, ',')) == NULL){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "command from first set must have two operands separated with comma.");
        return;
    }
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] == ','){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "must start with an operand and not with ',' sign.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])) && line[*index + len] != ','; len++);
    /* Store the first operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] = analyze_operand(operand, tree);
    /* Check that the first operand is valid */
    if(tree->lex_line_option == lex_line_error){
        strcpy(tree->lex_error, "First operand is illegal.");
        free(operand);
        return;
    }
    else{
        /* if first operand is a immediate address*/
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_immed){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].immed_val = atoi(operand);
        }
        /* if first operand is a label*/
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_label){
            strcpy(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].label_val, operand);
        }
        /* if first operand is a register*/
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[0] == lex_op_addr_type_register){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[0].reg_val = operand[2];
        }
    }
    free(operand);
    (*index) += len;
    MOVE_NEXT_TXT(line, *index)
    (*index)++; /* Skip the comma */
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] == ','){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Should be one comma after operand.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])) && line[*index + len] != ','; len++);
    /* Store the second operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] = analyze_operand(operand, tree);
    /* Check that the second operand is valid */
    if(tree->lex_line_option == lex_line_error){
        strcpy(tree->lex_error, "Second operand is illegal.");
        free(operand);
        return;
    }
    else{
        /* if second operand is a immediate address */
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_immed){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].immed_val = atoi(operand);
        }
        /* if second operand is a label */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_label){
            strcpy(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].label_val, operand);
        }
        /* if second operand is a register */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oat[1] == lex_op_addr_type_register){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_a.oatv[1].reg_val = operand[2] ;
        }
    }
    free(operand);
    (*index) += len;
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Should not have character after second operand");
        return;
    }
}

void store_set_b_operands(const char *line, int *index, struct lex_tree *tree){
    int len;
    char *operand;
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] == ','){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "must start with an operand and not with ',' sign.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' &&
                 line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    /* Store the operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    (*index) += len;
    MOVE_NEXT_TXT(line, *index)
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Only one operand is allowed in command second group.");
        free(operand);
        return;
    }

    tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat = analyze_operand(operand, tree);
    if(tree->lex_line_option == lex_line_error){
        free(operand);
        return;
    }
    else{
        /* if the operand is a immediate address */
        if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_immed){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.immed_val = atoi(operand);
        }
        /* if the operand is a label */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_label){
            strcpy(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.label_val, operand);
        }
        /* if the operand is a register */
        else if(tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oat == lex_op_addr_type_register){
            tree->line_inst_line_dir.inst_line.inst_sets.inst_set_b.oatv.reg_val = operand[2];
        }
        free(operand);
        return;
    }
}
