#include "macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Macro Macro;

/** Function to create a new node with the given data */
Macro* createMacro(const char* data1, const char* data2) { 
    Macro* newMacro = (Macro*)malloc(sizeof(Macro)); 
    if (newMacro) {
	newMacro->name = malloc(sizeof(char) * MAX_WORDS);
	newMacro->cont = malloc(sizeof(char) * 5000);
	if (!newMacro->name ||!newMacro->cont) {
	/* Todo on error*/
		printf("error:can't allocate memory for creating a new macro");
		return NULL;
	}
	else{
	 
         strcpy(newMacro->name, data1); 
        strcpy(newMacro->cont, data2); 
        newMacro->next = NULL;
        return newMacro;
        }
    } 
   printf("error:can't allocate memory for creating a new macro");
   return NULL;  
}

/* Function to insert a node at the end of the list */
void insertEnd(Macro** head, const char* data1, const char* data2) {
    Macro* newMacro = createMacro(data1, data2);
    if (newMacro == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    if (*head == NULL) {
        *head = newMacro;
    } else {
        Macro* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newMacro;
    }
}



/* Function to free memory allocated for the linked list */
void freeList(Macro* head) {
    while (head != NULL) {
        Macro* temp = head;
        head = head->next;
        free(temp);
    }
}



const char* getSecondVariable(Macro* list, const char* macroName) {
    Macro* current = list;
    while (current != NULL) {
        if (strcmp(current->name, macroName) == 0) {
            /* Return the content of the current macro */
            return current->cont;
        }
        current = current->next;
    }
    return NULL;  /* Return NULL if macroName is not found */
}

Macro* updateFirstVariable(Macro* list, const char* targetName, const char* newValue) {
    Macro* current = list;
    while (current != NULL) {
        if (strcmp(current->name, targetName) == 0) {
            strcpy(current->name, newValue);
            break;  /* Assuming you only want to update the first occurrence */
        }
        current = current->next;
    }
    return list;
}

void appendToSecondVariable(Macro* list, const char* targetName,  char* contentToAdd) {
    Macro* current = list;
    while (current != NULL) {
        if (strcmp(current->name, targetName) == 0) {
            strcat(current->cont, contentToAdd);
            break;  /* Assuming you only want to update the first occurrence */
        }
        current = current->next;
    }
}

int isWordInFirstVariable(const Macro* list, const char* targetWord) {
    const Macro* current = list;
    while (current != NULL) {
        if (strstr(current->name, targetWord) != NULL) {
            return 1;  /* Target word found in the first variable */
        }
        current = current->next;
    }
    return 0;  /* Target word not found in any first variable */
}

