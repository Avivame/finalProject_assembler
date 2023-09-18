#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "consts.h"
#include "data_structure.h"
#include "lexical_analyzer.h"


bool is_number_operand(const char *operand) {
    while (*operand != '\0') {
        if (!isdigit(*operand) && operand[0] != '-') {
            /* operand contains character that are not digits */
            return FALSE;  
        }
        operand++;
    }
    /* operand contains only digits */
    return TRUE;  
}


bool is_label_operand(char *word){
    char label[MAX_LABEL_SIZE];
    int i;
    const char* reg_comm_words[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6",
                                    "@r7","and", "mov", "cmp", "add", "sub", "not",
                                    "clr","lea","inc", "dec", "jmp", "bne", "red",
                                    "prn","jsr", "rts", "stop"};
    const int num_reg_comm_words = 25;

    if (strlen(word) > MAX_LABEL_SIZE){
        return FALSE;
    }

    /* get the word without ':' sign */
    if(word[strlen(word) -1] == ':'){
        for(i=0;i<strlen(word)-1;i++){
            label[i] = word[i];
        }
        label[i] = '\0';
    }
    else{ /* label is a operand in instruction line */
        for(i=0;i<strlen(word);i++){
            label[i] = word[i];
        }
        label[i] = '\0';
    }

    /* Check if starting with alpha character */
    if(!isalpha(label[0])){
        return FALSE;
    }

    /* Check that the label isn't a register/command word */
    for(i=0; i<num_reg_comm_words; i++){
        if(strcmp(label, reg_comm_words[i]) == 0){
            return FALSE;
        }
    }

    /* Check if the all characters are alphanumeric in the label */
    for(i=0;label[i] != '\0'; i++){
        if(!isalnum(label[i])){
            return FALSE;
        }
    }
    return TRUE;
}


enum operand_addr_type analyze_operand(char *operand, struct  lex_tree *tree){
    char *endptr;
    long value;
    char *value_str;
    int i;
    if (strlen(operand) == 0) {
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Missing operand.");
        return lex_op_addr_type_error;
    }

    if (is_number_operand(operand)) {
        /* Immediate number */
        value_str = &operand[1];

        /* Check that the rest of the string only contains digits and optional '-' sign */
        for (i = 0; i < strlen(value_str); i++) {
            if (!isdigit(value_str[i]) && value_str[i] != '-') {
                /* there are characters that are not digits */
                tree->lex_line_option = lex_line_error;
                strcpy(tree->lex_error, "an immediate operand needs to have only digits characters");
                return lex_op_addr_type_error;
            }
        }

        /* Check that the entire operand is a valid integer */

        value = strtol(value_str, &endptr, 10);
        if (*endptr != '\0') {
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "Invalid integer.");
            return lex_op_addr_type_error;
        }

        if (value >= -(1 << 9) && value < (1 << 9)) {
            /* Valid immediate operand */
            return lex_op_addr_type_immed;
        } else {
            /* Error: value out of range */
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "Out of range integer.");
            return lex_op_addr_type_error;
        }

    }else if(strlen(operand) == 2 && operand[0] == 'r' && isdigit(operand[1])){
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Register operand have to start with '@' sign.");
        return lex_op_addr_type_error;
    } else if (strlen(operand) == 3 && operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2])) {
        int register_num = operand[2] - '0';
        if (register_num >= 0 && register_num <= 7) {
            return lex_op_addr_type_register;
        } else {
            tree->lex_line_option = lex_line_error;
            strcpy(tree->lex_error, "Invalid register number - should be number between 1-7");
            return lex_op_addr_type_error;
        }
    }else if (is_label_operand(operand)) {
        return lex_op_addr_type_label;
    } else {
        tree->lex_line_option = lex_line_error;
        strcpy(tree->lex_error, "Illegal operand.");
        return lex_op_addr_type_error;
    }
}


char *str_allocate_cat(char *first_str, char* second_str) {
    char *str = (char *)malloc(strlen(first_str) + strlen(second_str) + 1);
    strcpy(str, first_str);
    strcat(str, second_str);
    return str;
}


void print_error_message(line_info *line, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("\033[0;31m");
    printf("ERROR: ");
    vprintf(format, args);
    printf("\033[0m");
    printf(" in the file: ");
    printf("\033[0;33m");
    printf("%s", line->filename);
    printf("\033[0m");
    printf(" in line: ");
    printf("\033[0;33m");
    printf("%d\n", line->line_number);
    printf("\033[0m");
    va_end(args);
}


void print_data_word(data_word word){
    int i;
    for (i = 11; i >= 0; i--) {
        putchar((word.data >> i) & 1 ? '1' : '0');
    }
    putchar('\n');
}


void print_machine_word(machine_word machine_word) {
    int i =0;
    unsigned int value;
    if(machine_word.word.f_word != NULL){
        value = *(unsigned int*) machine_word.word.f_word;
    }
    if (machine_word.label != NULL){
        printf("%s", machine_word.label);
    }
    else {
        for (i = 11; i >= 0; i--) {
            printf("%d", (value >> i) & 1);
        }
    }
    printf("\n");
}


void free_code_image(machine_word code_image[], long ic) {
    int i;
    for (i = 0; i < ic; i++) {
        if (code_image[i].label != NULL){
            free(code_image[i].label);
            code_image[i].label = NULL;
        }
        if(code_image[i].word.f_word != NULL) {
            free(code_image[i].word.f_word);
            code_image[i].word.f_word = NULL;
        }
        else if (code_image[i].word.id_word != NULL) {
            free(code_image[i].word.id_word);
            code_image[i].word.id_word = NULL;
        }
        if (code_image[i].word.r_word != NULL) {
            free(code_image[i].word.r_word);
            code_image[i].word.r_word = NULL;
        }
    }
}


void update_data_symbols_address(symbol_table *table, long ic) {
    int i;
    for (i = 0; i < TABLE_SIZE; ++i) {
        symbol_table_entry *entry = table->entries[i];
        while (entry != NULL) {
            if (entry->symbol_type == DATA_SYMBOL) {
                entry->symbol_addr += ic;
            }
            entry = entry->next;
        }
    }
}
