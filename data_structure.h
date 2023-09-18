#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "consts.h"

#define TABLE_SIZE 64

/* Define an entry in a symbols table */
typedef struct symbol_table_entry{
    char *symbol;
    int symbol_addr;
    symbol_type symbol_type;
    struct symbol_table_entry *next;
} symbol_table_entry;


/* Define a symbol table */
typedef struct symbol_table{
    symbol_table_entry **entries;
} symbol_table;


/*
    The hash function for the function
    @param key The key of the value to be inserted to the table
    @return A value from 0 to TABLE_SIZE
*/
unsigned int hash(const char *key);


/*
    Creates an empty Symbol Table
    @return A pointer to the table
*/
symbol_table *create_symbol_table(void);


/*
    Allocating memory to the symbol name and assigning the symbol address and type.
    @param key Pointer to the key of the pair.
    @param data Pointer to the data of the pair.
    @return A table_entry object with next set to null.
*/
symbol_table_entry *symbol_table_pair(const char *symbol, int symbol_addr, symbol_type symbol_type);


/*
    Add an entry to the table
    @param hashtable Pointer to the table to add the entry to.
    @param key Pointer to the key of the data.
    @param data Pointer to the data.
*/
void add_entry(symbol_table *hashtable, const char *symbol, int symbol_addr, symbol_type symbol_type);



/*
    Gets a symbol entry from a macros table
    @param hashtable A pointer to the macros table
    @param symbol The symbol to get its entry
    return A pointer to the symbol entry
 */
symbol_table_entry *get_from_symbol_table(symbol_table *hashtable, const char *symbol);


/*
    Prints a symbol table
    @param hashtable A pointer to the table to be printed
*/
void print_symbol_table(symbol_table *hashtable);


/*
    Frees the memory of a symbol table
    @param hashtable A pointer to the table to be freed
*/
void free_symbol_table(symbol_table *hashtable);

#endif
