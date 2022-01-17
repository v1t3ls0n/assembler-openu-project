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
    void *next;
    union
    {
        SymbolData s;
        MacroData m;
    } val;
} Item;

static Item *symbols[HASHSIZE];
static Item *macros[HASHSIZE];
