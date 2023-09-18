#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_structure.h"
#include "consts.h"



unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);
    /* Do several rounds of multiplication */
    for (i=0; i < key_len; i++){
        value = value * 37 + key[i];
    }
    value = value % TABLE_SIZE; /* Make sure value is between 0 and TABLE_SIZE */

    return value;
}

symbol_table *create_symbol_table(void){
    /* Allocate memory for table*/
    symbol_table *hashtable = malloc(sizeof(symbol_table));

    /* Allocate memory for the entries */
    hashtable->entries = malloc(sizeof(symbol_table_entry) * TABLE_SIZE);
    /* Initializing all the entries to be null */
    {
        int i=0;
        for (i=0;i<TABLE_SIZE; i++){
            hashtable->entries[i] = NULL;
        }
    }
    return hashtable;
}

symbol_table_entry *symbol_table_pair(const char *symbol, int symbol_addr, symbol_type  symbol_type){
    /* Allocate memory for the entry */
    symbol_table_entry *entry = malloc(sizeof(symbol_table_entry));
    entry->symbol = malloc(strlen(symbol) + 1);
    entry->symbol_addr = symbol_addr;
    entry->symbol_type = symbol_type;

    /* Get the value of the key & the data. */
    strcpy(entry->symbol, symbol);
    entry->next = NULL;

    return entry;
}

/* This add_entry function is used to add or update entries in the symbol table data structure.
 * If an entry with the same symbol already exists in the table, it updates the existing entry with the new data.
 * Otherwise, it adds a new entry to the table.
 * This way, the function maintains a symbol table with unique symbols and their associated attributes.*/
void add_entry(symbol_table *hashtable, const char *symbol, int symbol_addr, symbol_type symbol_type){
    unsigned int bucket = hash(symbol);
    symbol_table_entry *prev;
    symbol_table_entry *entry = hashtable -> entries[bucket];
    if (entry == NULL){
        hashtable->entries[bucket] = symbol_table_pair(symbol, symbol_addr, symbol_type);
        return;
    }

    /* Iterate through each entry to until the end is reach or key match found*/
    while (entry != NULL) {
        if(strcmp(entry->symbol, symbol) == 0) {
            /* Match found - Will replace the existing key with new one */
            free(entry->symbol);

            entry->symbol = malloc(strlen(symbol) + 1);
            entry->symbol_addr = symbol_addr;
            entry->symbol_type = symbol_type;

            strcpy(entry->symbol, symbol);
            return;
        }
        /* Iterate to the next node */
        prev = entry;
        entry = prev->next;
    }
    /* Reached the end of the linked list, add another node with the pair */
    prev->next = symbol_table_pair(symbol, symbol_addr, symbol_type);
}

symbol_table_entry *get_from_symbol_table(symbol_table *hashtable, const char *symbol) {
    unsigned int bucket = hash(symbol);

    /* try to find a valid bucket */
    symbol_table_entry *entry = hashtable->entries[bucket];

    /* no bucket means no entry */
    if (entry == NULL) {
        return NULL;
    }

    /* walk through each entry in the bucket, which could just be a single thing */
    while (entry != NULL) {
        /* return symbol if found */
        if (strcmp(entry->symbol, symbol) == 0) {
            return entry;
        }

        entry = entry->next;
    }

    /* reaching here means there were >= 1 entries but no key match */
    return NULL;
}


void print_symbol_table(symbol_table *hashtable) {
    int i;
    for (i=0; i < TABLE_SIZE; ++i) {
        symbol_table_entry *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        for(;;) {
            printf("%s[%d] in %d\n", entry->symbol, entry->symbol_type, (entry->symbol_addr) + 100);
            if (entry->next == NULL) {
                break;
            }
            entry = entry->next;
        }
    }
}

void free_symbol_table(symbol_table *hashtable) {
    int i;
    /* free entries */
    for (i=0; i < TABLE_SIZE; i++) {
        if (hashtable->entries[i] != NULL) {
            symbol_table_entry *entry = hashtable->entries[i];
            while (entry != NULL) {
                symbol_table_entry *prev = entry;
                entry = entry->next;
                free(prev->symbol);
                free(prev);
            }
        }
    }

    /* free table */
    free(hashtable->entries);
    free(hashtable);
}
