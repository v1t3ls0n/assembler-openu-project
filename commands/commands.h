#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".external"
#define A 0x4
#define R 0x2
#define E 0x1
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

typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrOptions;

typedef const struct
{
    unsigned int opMachineCodeHex;
    unsigned int opcode : 4;
    unsigned int funct : 4;
    char keyword[4];
    AddrOptions src;
    AddrOptions des;
} Command;
Command commands[] = {
    {0x0001, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

typedef struct
{
    unsigned int POS_A : 4;
    unsigned int POS_B : 4;
    unsigned int POS_C : 4;
    unsigned int POS_D : 4;
    unsigned int POS_E : 4;
} Word;

void printObjectFile(Word obj[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(Word obj[], unsigned int ICF, unsigned int DCF);
char *getFirstWord(Command *cmd);
Command *getCommandByName(char *s);
Bool isLabelNameLegal(char *s);
char *decToHex(int num);
char *hexToBin(char *hex);
