#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INST_SIZE 16

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
} Addr;

typedef struct
{
    unsigned int opcode : 4;
    unsigned int funct : 4;
    unsigned char keyword[4];
    Addr src;
    Addr des;
} Instruction;

const Instruction instructions[INST_SIZE] = {
    {0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {1, 0, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {2, 10, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {2, 11, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {4, 0, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {5, 10, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {5, 11, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {5, 12, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {5, 13, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {9, 10, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {9, 11, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {9, 12, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {12, 0, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {13, 0, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {14, 0, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {15, 0, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

const char regsName[] = {
    "r0",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
};
Instruction *getInstructionByName(char *s);
Bool isLabelNameLegal(char *s);