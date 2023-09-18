#include <stdio.h>
#include <stdlib.h>
#include "write_to_files.h"
#include "consts.h"
#include "utils.h"
#include "data_structure.h"



char *convert_to_base64(unsigned int value) {
    char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *base64_result = malloc(3);
    if (!base64_result) {
        return NULL;
    }

    base64_result[0] = base64_chars[(value >> 6) & 0x3F];
    base64_result[1] = base64_chars[value & 0x3F];
    base64_result[2] = '\0';

    return base64_result;
}


char *convert_signed_to_base64(int value) {
    unsigned int encoded_value;
    if (value < 0) {
        /* Convert negative value to 12-bit two's complement representation */
        encoded_value = (1 << 12) + (value & 0xFFF);
    } else {
        encoded_value = value & 0xFFF;
    }

    return convert_to_base64(encoded_value);
}


void write_obj_file(char *filename, machine_word code_image[], long ic, data_word data_image[], long dc) {
    FILE *file_des;
    int i;
    unsigned int value;
    char *obj_filename = str_allocate_cat(filename, ".ob");

    /* Try to create a new .ob file */
    file_des = fopen(obj_filename, "w");
    if (file_des == NULL) {
        printf("Couldn't open the file %s.\n", obj_filename);
        free(obj_filename);
        return;
    }

    fprintf(file_des, "\t\t%ld %ld\n", ic, dc); /* Print the header */
    /* Print the code image */
    for (i = 0; i < ic; i++) {

        /* Inside the loops where you print the code and data images */
        char *base64_value;

        if (code_image[i].word.f_word != NULL) {
            value = *(unsigned int *) code_image[i].word.f_word;
        }
        fprintf(file_des, "0%d\t\t", i + IC_INIT_VALUE);

        /* Calculate the base64 representation of the 12-bit value */
        base64_value = convert_signed_to_base64(value);
        fprintf(file_des, "%s\n", base64_value);
    }
    /* Print the data image */
    for (i = 0; i < dc; i++) {

        /* Inside the loops where you print the code and data images */
        char *base64_value;

        fprintf(file_des, "0%ld\t\t", i + ic + IC_INIT_VALUE);
        base64_value = convert_signed_to_base64(data_image[i].data);
        fprintf(file_des, "%s\n", base64_value);

    }
    /* Free the allocated memory */
    free(obj_filename);
    fclose(file_des);
}


void write_ent_file(char *filename, symbol_table *symbol_table){
    int i;
    FILE *file_des;
    char *ent_filename = str_allocate_cat(filename, ".ent");

    /* Try to create .ent file */
    file_des = fopen(ent_filename, "w");
    if(file_des == NULL){
        printf("Couldn't open the file %s.\n", ent_filename);
        free(ent_filename);
        return;
    }
    /* Print the entry symbols */

    for (i=0; i < TABLE_SIZE; ++i) {
        symbol_table_entry *entry = symbol_table->entries[i];

        if (entry == NULL) {
            continue;
        }

        for(;;) {
            if(entry->symbol_type == ENTRY_SYMBOL){
                fprintf(file_des, "%s\t\t%d\n", entry->symbol, (entry->symbol_addr) + 100);
            }
            if (entry->next == NULL) {
                break;
            }
            entry = entry->next;
        }
    }

    /* Free the allocated memory */
    free(ent_filename);
    fclose(file_des);
}



void write_ext_file(char *filename, symbol_table *symbol_table, machine_word code_image[], long ic){
    int num = 0; /* Counts the number of times used an .extern instruction */
    int i;
    FILE *file_des;
    char *ext_filename = str_allocate_cat(filename, ".ext");

    /* Check if there was an .extern instruction in the file. */
    for(i = 0; i < ic; i++){
        if(code_image[i].label != NULL){
            num++;
        }
    }
    if(num == 0){
        free(ext_filename);
        return;
    }
    /* Try to create .ext file */
    file_des = fopen(ext_filename, "w");
    if(file_des == NULL){
        printf("Couldn't open the file %s.\n", ext_filename);
        free(ext_filename);
        return;
    }
    /* Print the extern symbols */
    for(i=0; i < ic; i++){
        if(code_image[i].label != NULL){
            symbol_table_entry *entry = get_from_symbol_table(symbol_table, code_image[i].label);
            if(entry->symbol_type == EXTERN_SYMBOL){
                fprintf(file_des, "%s   %d\n",entry->symbol,  i + IC_INIT_VALUE);
            }
        }
    }
    /* Free the allocated memory */
    free(ext_filename);
    fclose(file_des);
}
