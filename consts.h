#ifndef CONSTS_H
#define CONSTS_H

#define MAX_FILE_NAME 80
/** Max size of a line in a program (.as file) */
#define MAX_LINE_LENGTH 80
/** Initial IC value */
#define IC_INIT_VALUE 100
/** The maximum size of a label */
#define MAX_LABEL_SIZE 31
/* The size of the memory of our computer */
#define MEMORY_SIZE 1024

/* Defines a boolean type (T/F) */
typedef enum bool{
    FALSE = 0, TRUE = 1
} bool;

/* Defines a register type - r0 to r7 & not a register */
typedef enum registers{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    NOT_REG = -1
} reg;

/* Defines the codes for the supported operations */
typedef enum op_codes{
    mov_op_code = 0,
    cmp_op_code,
    add_op_code,
    sub_op_code,
    not_op_code,
    clr_op_code,
    lea_op_code,
    inc_op_code,
    dec_op_code,
    jmp_op_code,
    bne_op_code,
    red_op_code,
    prn_op_code,
    jsr_op_code,
    rts_op_code,
    stop_op_code,
    error_op_code
} op_codes;

/* Defines the addressing methods for the operands */
typedef enum symbol_type{
    ENTRY_SYMBOL = 0,
    DATA_SYMBOL,
    EXTERN_SYMBOL,
    CODE_SYMBOL
} symbol_type;

/* Defines a first word in an instruction */
typedef struct first_word {
    unsigned int ARE: 2;
    unsigned int target_op_addr: 3;
    unsigned int op_code: 4;
    unsigned int source_op_addr: 3;
} first_word;

/* Defines a machine word of operand with direct or immediate addressing */
typedef struct imdt_drct_word {
    unsigned int ARE: 2;
    unsigned int operand: 10;
} imdt_drct_word;

/* Defines a machine word of registers operands - Can contain destination register & source register */
typedef struct register_word {
    unsigned int ARE: 2;
    unsigned int target_register: 5;
    unsigned int source_register: 5;
} register_word;

/* Defines a general machine word - first word, immediate or direct addressing word,
    register word or data word */
typedef struct machine_word {
    char *label; /* We'll put here label names in the first pass */
    union word {
        first_word *f_word;
        imdt_drct_word *id_word;
        register_word *r_word;
    } word;
} machine_word;

/* Defines a machine word containing data from .string or .data instruction */
typedef struct data_word {
    unsigned int data: 12;
} data_word;

/* Defines a line in a file and contains its info - file name, the number of the line and the content of the line */
typedef struct line_info{
    char *filename;
    int line_number;
    char *content;
} line_info;

#endif
