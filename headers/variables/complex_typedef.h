#include "constants.h"

/*Bit is is a special variable that we made to build with BinaryWord*/
typedef struct
{
    unsigned int on : 1;
} Bit;
/*BinaryWord is a struct for holding memory and data image in memory format*/
typedef struct
{
    Bit digit[BINARY_WORD_SIZE];
} BinaryWord;

/*Like BinaryWord, it represent a word with 5 hexa decimal characters in the order we will output to th .ob file*/
typedef struct
{
    unsigned int _A : 4;
    unsigned int _B : 4;
    unsigned int _C : 4;
    unsigned int _D : 4;
    unsigned int _E : 4;
} HexWord;

/*AddrMethodsOptions is a struct for addressing mehod, each member represent operand addressing method
and the value can be 0 or 1 and the meaning is to  tell us if the addressing method is legal/optional
For each operation's operand that we have in our assembly language.
*/
typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrMethodsOptions;

/*Operation is a struct for holding all the operation table data*/
typedef struct
{
    unsigned int op;        /*op- is the binary value of the first sixteen bits in the first word for each operation*/
    unsigned int funct : 4; /*the function code of the operation*/
    char keyword[4];        /*the keyword for the operation name*/
    AddrMethodsOptions src; /*legal addressing mehods for the source operand*/
    AddrMethodsOptions des; /*legal addressing mehods for the destination operand*/
} Operation;

/*ItemType is an identyfier parameter that we use in the hash table functions that both the macro table and the symbol table are built with*/
typedef enum
{
    Macro,
    Symbol
} ItemType;
/*DataType is an identyfier parameter that we use in the memory.c file functions in order to specify if a current memory word belongs to the data
image part or the instruction part of the resulted image*/
typedef enum
{
    Code,
    Data
} DataType;

/* MacroData is a struct that holds the additional unique data fields of each macro item in the macros hash table. The start member represents the starting index of the content of the macro in the source (.as) files
 The end member is the same as start, but for the ending of the macro content*/
typedef struct
{
    int start;
    int end;
} MacroData;

/* ExtPositionData is the nested singly linked list element in the ExtListItem that we just mentioned above.*/

typedef struct ExtPositionData
{
    unsigned base;
    unsigned offset;
    struct ExtPositionData *next;
} ExtPositionData;

/* ExtListItem is a typedef struct of a single external symbol name
that is used as operand in the assembly code. This item is the building block
of the external operands nested singly linked list. The nested list is a singly linked list
that holds ExtPositionData (base, offset) for any memory address the
external operand appeares to be used in the binary / hexa image we create. */
typedef struct ExtListItem
{
    char name[MAX_LABEL_LEN];
    ExtPositionData value;
    struct ExtListItem *next;
} ExtListItem;

/*Attributes for the type of symbol that are saved in the ymbol table*/
typedef struct
{
    unsigned int code : 1;
    unsigned int data : 1;
    unsigned int entry : 1;
    unsigned int external : 1;
} Attributes;

/*SymbolData is a struct that holds the additional unique data fields in the symbols hash table.
 The value member represents the  value of the memory address that the symbol appearse in.
The base and offset members represent the same address but in format that splits the memory address to
base address and offset address.
The attrs member is holding the type of the symbol (data, entry, external, code).
 */
typedef struct
{

    unsigned value;
    unsigned base;
    unsigned offset;
    Attributes attrs;
} SymbolData;

/*Item is the building block of the hash table data structure that both the symbols and macros tables are built with*/
typedef struct Item
{
    char name[MAX_LABEL_LEN];
    union
    {
        SymbolData s;
        MacroData m;
    } val;
    struct Item *next;
} Item;
