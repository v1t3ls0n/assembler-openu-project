#include "constants.h"

typedef struct
{
    unsigned int on : 1;
} Bit;

typedef struct
{
    Bit digit[BINARY_WORD_SIZE];
} BinaryWord;

typedef struct
{
    unsigned int _A : 4;
    unsigned int _B : 4;
    unsigned int _C : 4;
    unsigned int _D : 4;
    unsigned int _E : 4;
} HexWord;

typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrMethodsOptions;

typedef struct
{
    unsigned int op;
    unsigned int funct : 4;
    char keyword[4];
    AddrMethodsOptions src;
    AddrMethodsOptions des;
} Operation;

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
    Symbol,
} ItemType;

typedef enum
{
    Code,
    Data
} DataType;

typedef struct
{
    int start;
    int end;
} MacroData;
typedef struct
{
    unsigned base;
    unsigned offset;
    void *next;
} ExtPositionData;

typedef struct
{
    char *name;
    ExtPositionData value;
} ExtListItem;

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
