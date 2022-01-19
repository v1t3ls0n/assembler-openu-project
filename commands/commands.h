#include <ctype.h>
#include "table.h"
#define DATA ".data"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERNAL ".external"
#define A 0x4
#define R 0x2
#define E 0x1
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
#define IMMEDIATE_ADDR 0x00
#define DIRECT_ADRR 0x01
#define INDEX_ADDR 0x10
#define REGISTER_DIRECT_ADDR 0x11

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
    unsigned int POS_A : 4;
    unsigned int POS_B : 4;
    unsigned int POS_C : 4;
    unsigned int POS_D : 4;
    unsigned int POS_E : 4;
} EncodedWord;

void printObjectFile(EncodedWord words[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(EncodedWord words[], unsigned int ICF, unsigned int DCF);
Bool isLabelNameLegal(char *s);
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
EncodedWord *encodeIntNum(int num);
extern void printSymbolTable();
extern void printSymbolItem(Item *item);
extern void setSymbolData(Item *symbol, unsigned value, Attributes attrs);
extern void setMacroData(Item *macro, char *code);
extern void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
extern Command *getCommandByName(char *s);
extern char *getFirstWord(Command *cmd);