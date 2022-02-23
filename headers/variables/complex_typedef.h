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
