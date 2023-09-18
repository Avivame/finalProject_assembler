#include <stdlib.h>
#include <stdio.h>
#include "consts.h"
#include "utils.h"
#include "start.h"
#include "data_structure.h"
#include "lexical_analyzer.h"
#include "first_pass.h"
#include "second_pass.h"
#include "write_to_files.h"

/*
    This function gets a file name and process it to machine code.
    @param filename The file to be processed.
    @return TRUE if the file process succeeded and FALSE otherwise.
*/
bool process_file(char *filename){
    long ic = 0, dc = 0;
    data_word data_image[MEMORY_SIZE] = {0}; /* Contains the data image of the file */
    machine_word code_image[MEMORY_SIZE] = {0}; /* Contains the code image of the file */
    char *am_filename = NULL;
    char temp_line[MAX_LINE_LENGTH + 2];
    line_info line;
    symbol_table *symbol_table = NULL;
    FILE *file_desc = NULL;
    bool process_success = TRUE, extern_symbol = FALSE, entry_symbol = FALSE;
    char c;
    struct lex_tree line_structure;


    /* Spread the macros from the origin file */
    if(preAssembler(filename) == 1){
        printf("Couldn't spread the macros correctly into %s.as\n", filename);
        return FALSE;
    }

    /* Open the file created by the preprocessor */
    am_filename = str_allocate_cat(filename, ".am");
    file_desc = fopen(am_filename, "r");
    if(file_desc == NULL){
        printf("Couldn't open the file %s.\n", am_filename);
        free(am_filename);
        return FALSE;
    }

    /* Start first pass */
    symbol_table = create_symbol_table();
    line.filename = am_filename;
    line.content = temp_line;
    for(line.line_number=1; fgets(temp_line, MAX_LINE_LENGTH + 2, file_desc) != NULL; line.line_number++){
        /* Check if the line is too long */
        if (strchr(temp_line, '\n') == NULL && !feof(file_desc)){
            print_error_message(&line, "The line is too long, maximum size of an input line is 80 charachters.");
            process_success = FALSE;
            do {
                c = fgetc(file_desc);
            } while (c != '\n' && c != EOF);
        }
        else {
            line_structure = get_lex_tree_from_line(temp_line);
            if(line_structure.lex_line_option == lex_line_error){
                print_error_message(&line, line_structure.lex_error);
                process_success = FALSE;
            }
            else{
                if(line_structure.lex_line_option == lex_line_dir){
                    if(line_structure.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_entry)
                        entry_symbol = TRUE;
                    else if(line_structure.line_inst_line_dir.dir_line.lex_dir_option == lex_dir_extern)
                        extern_symbol = TRUE;
                }
                process_success = first_pass(line, symbol_table, &ic, &dc, data_image, code_image, &line_structure);
            }

        }

    }
    update_data_symbols_address(symbol_table, ic);

    /* Second pass */
    rewind(file_desc);
    if(process_success){
        for(line.line_number=1; fgets(temp_line, MAX_LINE_LENGTH + 2, file_desc) != NULL; line.line_number++){
            line_structure = get_lex_tree_from_line(temp_line); /* Syntax error detected in the first pass */
            process_success = second_pass(line_structure, symbol_table, line);
        }
        label_encoding(symbol_table, code_image, ic);
    }

    if(ic + dc > MEMORY_SIZE - 100){ /* First 100 memory cells reserved for the system. */
        print_error_message(&line, "The memory size is too small for the file.");
        process_success = FALSE;
    }

    /* Print files if process_success */
    if(process_success){
        write_obj_file(filename, code_image, ic, data_image, dc);
        if(entry_symbol) write_ent_file(filename, symbol_table);
        if(extern_symbol) write_ext_file(filename, symbol_table, code_image, ic);
    }


    /* Free all the allocated memory */
    free(am_filename);
    fclose(file_desc);
    free_symbol_table(symbol_table);
    free_code_image(code_image, ic);
    return TRUE;
}


int main(int argc, char *argv[])
{
    int i;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > MAX_FILE_NAME) {
            fprintf(stderr, "Filename %s is too long (max %d characters)\n",
                    argv[i], MAX_FILE_NAME);
            continue;
        }
        process_file(argv[i]);
    }
    return 0;
}
