
#define BINARY_WORD_SIZE 20
#define REGS_SIZE 16
#define OP_SIZE 16
#define RAM_MEMORY_SIZE 8192
#define MAX_LABEL_LEN 31
#define MAX_LINE_LEN 81
#define HASHSIZE RAM_MEMORY_SIZE / MAX_LABEL_LEN
#define A 0x40000
#define R 0x20000
#define E 0x10000
#define IMMEDIATE_ADDR 0x00
#define DIRECT_ADRR 0x01
#define INDEX_ADDR 0x10
#define REGISTER_DIRECT_ADDR 0x11

#define R0 "r0"
#define R1 "r1"
#define R2 "r2"
#define R3 "r3"
#define R4 "r4"
#define R5 "r5"
#define R6 "r6"
#define R7 "r7"
#define R8 "r8"
#define R9 "r9"
#define R10 "r10"
#define R11 "r11"
#define R12 "r12"
#define R13 "r13"
#define R14 "r14"
#define R15 "r15"

#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".extern"

#define _TYPE_DATA -11
#define _TYPE_STRING -22
#define _TYPE_ENTRY -33
#define _TYPE_EXTERNAL -44
#define _TYPE_CODE -55