#include "constants.h"

typedef struct
{
    unsigned int on : 1;
} Bit;
typedef struct
{
    Bit digit[20];
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
    union
    {
        HexWord *hex;
        BinaryWord *binary;
    } value;
    void *next;
} Word;

typedef struct
{
    Word *head;
    Word *tail;
} MemoryStack;

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

typedef struct
{
    const unsigned int op;
    const unsigned int funct : 4;
    const char keyword[4];
    const AddrMethodsOptions src;
    const AddrMethodsOptions des;
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