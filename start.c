#include "start.h"
#include "macro.h"


int preAssembler(const char* filename) {
    char fname1[MAX_LINE_LENGTH];
    char fname2[MAX_LINE_LENGTH];

    /* takes file name */
    strcpy(fname1, filename);
    strcpy(fname2, filename);
    /* concatenate the string ".am" to the fname2 */
    strncat(fname1, ".as", sizeof(fname1) - strlen(fname1) - 1);
    strncat(fname2, ".am", sizeof(fname2) - strlen(fname2) - 1);
    Macro* macroList = NULL;
    macroList = createMacro("", "");

    int i, j;
    for (i = 0; i < MAX_WORDS; i++) {
        for (j = 0; j < MAX_WORD_LENGTH; j++) {
            words[i][j] = '\0';
        }
    }
    char *line;
    char macName[MAX_LINE_LENGTH];
    FILE *fpr = openFileForReading(fname1);
    FILE *fpw = openFileForWriting(fname2);
    line = get_next_line(fpr);
    while(line != NULL) {
        getArrayFromLine(line);
        if (strcmp(words[0], BEGGING_OF_MACRO) == 0&& strcmp(words[0],"\0")!=0 && words[0][0]!=';') {
        /*if the macro isn't correct since another word after macro name*/
            if (words[2][0] != '\0') {
                free(fpr);
                free(fpw);
                return 1;
            }
            /*thers no macro name*/
            if (words[1][0] == '\0') {
                free(fpr);
                free(fpw);
                return 1;
            }
            /*the macro name is a name of a function*/
            if (is_macro_name_of_func() == 1) {
                free(fpr);
                free(fpw);
                return 1;
            }
            /*if the macro name is ok odd it to macro table*/
            strcpy(macName , words[1]);
            updateFirstVariable(macroList, "", macName);
            /*if line is   empty or instruction*/
            line = get_next_line(fpr);
            getArrayFromLine(line);
            /*if the line is a not needed*/
            if(strcmp(words[0],"\0")==0 || words[0][0]==';'){
                line = get_next_line(fpr);
                getArrayFromLine(line);
            }
            int flag = 1;
            /*copy all macro content to table*/
            while (strcmp(words[0], END_OF_MACRO) != 0 && *line != EOF) {
		/*if line not needed*/
                if (words[0][0] == '\0' || words[0][0] == ';')
                {
                line = get_next_line(fpr);
                getArrayFromLine(line);
                }
                else
                {
                	/*add secont line to macro's content whith enter between each line and space after each word*/
                    if (flag != 1) {
                    appendToSecondVariable(macroList, macName, "\n");
                    }
                    flag = 0;
                    int z = 0;
                    while (words[z][0] != '\0') {
                        appendToSecondVariable(macroList, macName, words[z]);
                        appendToSecondVariable(macroList, macName, " ");
                        z++;
                    }
                    line = get_next_line(fpr);
                    getArrayFromLine(line);
                    FixLine
                }
            }
            line = get_next_line(fpr);
            /*if in endmacro line thers another word error*/
            if (words[1][0] != '\0' ) {
                free(fpr);
                free(fpw);
                return 1;
            }
            insertEnd(&macroList, "", "");
        }

        else if(strcmp(words[0],"\0")==0 || words[0][0]==';')
        {
                line = get_next_line(fpr);
        }
        else if(isWordInFirstVariable(macroList, words[0]) == 1) {
            if(words[1][0] != '\0') {
                free(fpr);
                free(fpw);
                return 1;
            }
            char* cont = getSecondVariable(macroList, words[0]) ;
            fprintf(fpw, "%s\n", cont);
            /* placeSecondVariableInFile(macroList, words[0], fpw); */
           line = get_next_line(fpr);
        }
        else {
            fprintf(fpw, "%s\n", line);
            line = get_next_line(fpr);
        }

    }
    freeList(macroList);
    fclose(fpr);
    fclose(fpw);
    return 0;
}


/* get an array of words from a line */
void getArrayFromLine(const char *sentence) {
    /* char words[MAX_WORDS][MAX_WORD_LENGTH]; */
    char copy[MAX_WORD_LENGTH];  
    int wordCount = 0;
    int wordIndex = 0;
    int i = 0;

    while (sentence[i] != '\0') {
        if (sentence[i] == ' ' || sentence[i] == '\t' || sentence[i] == '\n') {
            copy[wordIndex] = '\0';
            if (wordIndex > 0) {
                strcpy(words[wordCount], copy);
                wordCount++;
                wordIndex = 0;
            }
        } else {
            copy[wordIndex] = sentence[i];
            wordIndex++;
        }

        if (wordCount >= MAX_WORDS - 1) {
            break; 
        }

        i++;
    }

    if (wordIndex > 0) {
        copy[wordIndex] = '\0';
        strcpy(words[wordCount], copy);
        wordCount++;
    }
    /*add end of line*/
words[wordCount][0] = '\0';  
    

}

char* get_next_line(FILE* file) {
    static char line[80];
    if (fgets(line, 80, file)) {
       
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        return line;
    }
    return NULL; 
    }

/* open file for reading only */
FILE* openFileForReading(const char *fileName) {
    FILE *fpr;
    fpr = fopen(fileName,"r");/*Attempt to open the file specified by fileName for reading*/
    if(fpr == NULL) {
        printf("error: cant open the file: %s \n \n" , fileName);
        exit(1);
    }
    return fpr;
}

/* open file for writing only */
FILE* openFileForWriting(const char *fileName) {
    FILE *fpw;
    fpw = fopen(fileName,"w");/*Attempt to open the file specified by fileName for writing*/
    if(fpw == NULL) {
        printf("error: cant open the file: %s \n \n" , fileName);
        exit(1);
    }
    return fpw;
}
/*checks that the macro name isnt a command word */
int is_macro_name_of_func() {
    if(strcmp(words[1], "mov") == 0 || strcmp(words[1], "cmp") == 0 ||
       strcmp(words[1], "add") == 0 || strcmp(words[1], "sub") == 0 ||
       strcmp(words[1], "not") == 0 || strcmp(words[1], "clr") == 0 ||
       strcmp(words[1], "lea") == 0 || strcmp(words[1], "inc") == 0 ||
       strcmp(words[1], "dec") == 0 || strcmp(words[1], "jmp") == 0 ||
       strcmp(words[1], "bne") == 0 || strcmp(words[1], "red") == 0 ||
       strcmp(words[1], "jsr") == 0 || strcmp(words[1], "prn") == 0 ||
       strcmp(words[1], "rts") == 0 || strcmp(words[1], "stop") == 0) {
        return 1;
    }
    return 0;
}

