#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".external"
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
    unsigned int POS_A : 4;
    unsigned int POS_B : 4;
    unsigned int POS_C : 4;
    unsigned int POS_D : 4;
    unsigned int POS_E : 4;
} EncodedWord;

typedef struct
{
    unsigned int are : 4;
    unsigned int funct : 4;
    unsigned int srcReg : 4;
    unsigned int srcAddr : 2;
    unsigned int desReg : 4;
    unsigned int desAddr : 2;
} secondWordState;

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
} AddrMethodsOptions;

typedef const struct
{
    unsigned int opMachineCodeHex;
    unsigned int opcode : 4;
    unsigned int funct : 4;
    char keyword[4];
    AddrMethodsOptions src;
    AddrMethodsOptions des;
} Command;

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
void setSymbolData(Item *symbol, unsigned value, Attributes attrs);
void setMacroData(Item *macro, char *code);
void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
