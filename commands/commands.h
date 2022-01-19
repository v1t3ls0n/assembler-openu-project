#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".external"

#define A "0x4"
#define R "0x2"
#define E "0x1"

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

typedef enum
{
    False,
    True
} Bool;

const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrOptions;

typedef const struct
{
    unsigned char opMachineCodeHex[8];
    unsigned int opcode : 4;
    unsigned int funct : 4;
    char keyword[4];
    AddrOptions src;
    AddrOptions des;
} Command;

Command commands[] = {
    {"0x0001", 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"0x0002", 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {"0x0004", 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"0x0004", 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"0x0010", 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {"0x0020", 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"0x0020", 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"0x0020", 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"0x0020", 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"0x0200", 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"0x0200", 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"0x0200", 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"0x1000", 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"0x2000", 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {"0x4000", 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"0x8000", 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};
char *getFirstWordByCommand(Command *cmd);
Command *getCommandByName(char *s);
Bool isLabelNameLegal(char *s);
char *hexToBin(char *hex);

/*

typedef enum
{
    MOV = 0x1,
    CMP = 0x2,
    SUB = 0x4,
    ADD = 0x4,
    LEA = 0x10,
    CLR = 0x20,
    NOT = 0x20,
    INC = 0x20,
    DEC = 0x20,
    JMP = 0x200,
    BNE = 0x200,
    JSR = 0x200,
    RED = 0x1000,
    PRN = 0x2000,
    RTS = 0x4000,
    STOP = 0x8000,
} OP_MACHINE_CODE_HEX;


Command commands[] = {
    {MOV, 0x0, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {CMP, 0x1, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {ADD, 0x2, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {SUB, 0x2, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {LEA, 0x4, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {CLR, 0x5, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {NOT, 0x5, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {INC, 0x5, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {DEC, 0x5, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {JMP, 0x9, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {BNE, 0x9, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {JSR, 0xA, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {RED, 0xC, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {PRN, 0xD, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {RTS, 0xE, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {STOP, 0xF, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};


#define MOV = 0x1
#define CMP = 0x2
#define SUB = 0x4
#define ADD = 0x4
#define LEA = 0x10
#define CLR = 0x20
#define NOT = 0x20
#define INC = 0x20
#define DEC = 0x20
#define JMP = 0x200
#define BNE = 0x200
#define JSR = 0x200
#define RED = 0x1000
#define PRN = 0x2000
#define RTS = 0x4000
#define STOP = 0x8000

#define A = 0x40000
#define R = 0x20000
#define E = 0x10000


typedef const struct
{
    char keyword[3];
    unsigned int machineCodeHex;
} Reg;
Reg regs[] = {

    {R0, 0x0},
    {R1, 0x1},
    {R2, 0x2},
    {R3, 0x3},
    {R4, 0x4},
    {R5, 0x5},
    {R6, 0x6},
    {R7, 0x7},
    {R8, 0x8},
    {R9, 0x9},
    {R10, 0xA},
    {R11, 0xB},
    {R12, 0xC},
    {R13, 0xD},
    {R14, 0xE},
    {R15, 0xF},


Command commands[] = {
    {MOV, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {CMP, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {ADD, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {SUB, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {LEA, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {CLR, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {NOT, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {INC, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {DEC, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {JMP, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {BNE, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {JSR, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {RED, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {PRN, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {RTS, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {STOP, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};
};
*/
