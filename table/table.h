#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASHSIZE 101

typedef struct
{
    unsigned int external : 1;
    unsigned int code : 1;
    unsigned int entry : 1;
    unsigned int data : 1;
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

static Item *symbols[HASHSIZE];
static Item *macros[HASHSIZE];

unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
void printSymbolItem(Item *item);
void setSymbolData(Item *symbol, unsigned value, Attributes attrs);
void setMacroData(Item *macro, char *code);
