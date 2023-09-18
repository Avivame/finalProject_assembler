
#ifndef MACRO_H
#define MACRO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_WORDS 80
 
typedef struct Macro {
    char *name; /* First data string */
    char *cont; /* Second data string */
    struct Macro* next; /* Pointer to the next node */
} Macro;



/**
 * Retrieves the content of a specified macro from a linked list.
 *
 * This function traverses the linked list of Macro nodes to find the macro with the
 * specified macroName. If the macro is found, its content is returned as a pointer to
 * the content string (char array). If the specified macroName is not found in the list,
 * the function returns NULL.
 *
 * @param list        A pointer to the head node of the linked list of Macro nodes.
 * @param macroName   The name of the macro whose content is to be retrieved.
 * @return            A pointer to the content of the specified macro, or NULL if not found.
 */
const char* getSecondVariable(Macro* list, const char* macroName);


/**
 * Checks if a target word is present in the first variable of any Macro node in a linked list.
 *
 * This function traverses the linked list of Macro nodes and searches the first variable
 * (name) of each node to determine if the targetWord is present. If the target word is
 * found in the first variable of any node, the function returns 1. If the target word is
 * not found in any node, the function returns 0.
 *
 * @param list        A pointer to the head node of the linked list of Macro nodes.
 * @param targetWord  The target word to search for in the first variable of nodes.
 * @return            1 if target word is found, 0 if not found in any first variable.
 */
int isWordInFirstVariable(const Macro* list, const char* targetWord);
/**
 * Appends content to the second variable of the first occurrence of a specified macro.
 *
 * This function searches through the linked list of Macro nodes to find the first
 * occurrence of the macro with the specified targetName. If the macro is found, the
 * contentToAdd is appended to the content (second variable) of the target macro using
 * the strcat function. Only the first occurrence is updated, and the loop terminates
 * after the update is performed.
 *
 * @param list          A pointer to the head node of the linked list of Macro nodes.
 * @param targetName    The name of the macro to which content is to be appended.
 * @param contentToAdd  The content to be appended to the target macro's content.
 */
void appendToSecondVariable(Macro* list, const char* targetName,  char* contentToAdd);
/**
 * Creates a new Macro node with the given data.
 *
 * This function dynamically allocates memory for a new Macro node,
 * initializes its fields with the provided data, and returns a pointer
 * to the newly created node.
 *
 * @param data1 The name of the macro.
 * @param data2 The content of the macro.
 * @return A pointer to the newly created Macro node, or NULL if memory allocation fails.
 */
Macro* createMacro(const char* data1, const char* data2);
/**
 * Inserts a new node at the end of a linked list of Macro nodes.
 *
 * This function creates a new Macro node with the provided data1 and data2, and inserts
 * it at the end of the linked list pointed to by the head pointer. If memory allocation
 * for the new node fails, an error message is printed to stderr. If the list is initially
 * empty (head is NULL), the new node becomes the first node. Otherwise, the function
 * iterates through the list to find the last node and appends the new node to the end.
 *
 * @param head      A pointer to a pointer that points to the head of the linked list.
 * @param data1     The data to be stored in the first field of the new Macro node.
 * @param data2     The data to be stored in the second field of the new Macro node.
 */
void insertEnd(Macro** head, const char* data1, const char* data2);

/**
 * Updates the name of the first occurrence of a specified macro in a linked list.
 *
 * This function searches through the linked list of Macro nodes to find the first
 * occurrence of the macro with the specified targetName. If the macro is found, its
 * name is updated to the provided newValue using the strcpy function. Only the first
 * occurrence is updated, and the loop terminates after the update is performed.
 *
 * @param list        A pointer to the head node of the linked list of Macro nodes.
 * @param targetName  The name of the macro whose name is to be updated.
 * @param newValue    The new value to replace the name of the target macro.
 * @return            A pointer to the updated linked list.
 */
Macro* updateFirstVariable(Macro* list, const char* targetName, const char* newValue);
/**
 * Frees the memory allocated for a linked list of Macro nodes.
 *
 * This function releases the memory for each node in the linked list, starting from
 * the head node. It iterates through the list, freeing the memory for each node one
 * by one, until the end of the list is reached (head pointer becomes NULL).
 *
 * @param head  A pointer to the head node of the linked list to be freed.
 */
void freeList(Macro* head);


#endif 
