#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "flags.h"
#define CMD_AND_REGS_SIZE 16
#define RAM_MEMORY_SIZE 8192
#define MAX_LABEL_LEN 31
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

typedef struct
{
    unsigned int _A : 4;
    unsigned int _B : 4;
    unsigned int _C : 4;
    unsigned int _D : 4;
    unsigned int _E : 4;
} EncodedWord;

typedef struct
{
    unsigned address;
    EncodedWord value;
    void *next;
} Word;

typedef struct
{
    unsigned int are : 4;
    unsigned int funct : 4;
    unsigned int srcReg : 4;
    unsigned int srcAddr : 2;
    unsigned int desReg : 4;
    unsigned int desAddr : 2;
} secondWordState;

typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrMethodsOptions;

typedef const struct
{
    unsigned int op;
    unsigned int funct : 4;
    char keyword[4];
    AddrMethodsOptions src;
    AddrMethodsOptions des;
} Command;

Command commands[] = {
    {0x0001, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

typedef struct
{
    unsigned int code : 1;
    unsigned int data : 1;
    unsigned int entry : 1;
    unsigned int external : 1;
} Attributes;

typedef enum
{
    Macro,
    Symbol
} ItemType;

typedef enum
{
    Code,
    Data
} DataType;

typedef struct
{
    char *code;
} MacroData;

typedef struct
{

    unsigned value;
    unsigned base;
    unsigned offset;
    Attributes attrs;
} SymbolData;

typedef struct
{
    char *name;
    union
    {
        SymbolData s;
        MacroData m;
    } val;
    void *next;
} Item;

unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
void printSymbolItem(Item *item);
Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
void updateSymbol(char *name, int newValue);
char *getMacroCodeValue(char *s);
void addMacro(char *name, char *code);
void verifyLabelNaming(char *s);

/*
const struct
{
    unsigned int op;
    unsigned int funct : 4;
    char keyword[4];
    AddrMethodsOptions src;
    AddrMethodsOptions des;
} cmds[] = {
    {0x0001, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

 */

/*
     indexes numbers of the positions inside the original file
    this attitude will make this program use less space and so it is more effiecient memorywise.
typedef struct
{
int start;
int end;
}
MacroDataV2;
*/
