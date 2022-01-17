#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASHSIZE 101
typedef enum
{
    external,
    code,
    entry,
    data
} Attribute;

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
    Attribute *attrs;
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
void setSymbolData(Item *symbol, unsigned value, Attribute *attrs);
void setMacroData(Item *macro, char *code);