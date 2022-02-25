
#include "data.h"
/* Shared global State variables*/

State globalState = firstRun;
Error currentError = noErrors;
Item *symbols[HASHSIZE] = {0};
Item *macros[HASHSIZE] = {0};

Operation operations[OP_SIZE] = {
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

/* Complex Struct Constant Variables:
Operation operations[OP_SIZE] =
   {0x0001, 0, "mov",1, 1, 1, 10, 1, 1, 1}
   {0x0002, 0, "cmp",1, 1, 1, 11, 1, 1, 1}
   {0x0004, 10, "add",1, 1, 1, 10, 1, 1, 1}
   {0x0004, 11, "sub",1, 1, 1, 10, 1, 1, 1}
   {0x0010, 0, "lea",0, 1, 1, 00, 1, 1, 1}
   {0x0020, 10, "clr",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 11, "not",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 12, "inc",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 13, "dec",0, 0, 0, 00, 1, 1, 1}
   {0x0200, 10, "jmp",0, 0, 0, 00, 1, 1, 0}
   {0x0200, 11, "bne",0, 0, 0, 00, 1, 1, 0}
   {0x0200, 12, "jsr",0, 0, 0, 00, 1, 1, 0}
   {0x1000, 0, "red",0, 0, 0, 00, 1, 1, 1}
   {0x2000, 0, "prn",0, 0, 0, 01, 1, 1, 1}
   {0x4000, 0, "rts",0, 0, 0, 00, 0, 0, 0}
   {0x8000, 0, "stop",0, 0, 0, 00, 0, 0, 0}
};
*/
/*
const struct
{
    unsigned int op;
    unsigned int funct : 4;
    char keyword[4];
    AddrMethodsOptions src;
    AddrMethodsOptions des;
} cmds[] =
   {0x0001, 0, "mov",1, 1, 1, 10, 1, 1, 1}
   {0x0002, 0, "cmp",1, 1, 1, 11, 1, 1, 1}
   {0x0004, 10, "add",1, 1, 1, 10, 1, 1, 1}
   {0x0004, 11, "sub",1, 1, 1, 10, 1, 1, 1}
   {0x0010, 0, "lea",0, 1, 1, 00, 1, 1, 1}
   {0x0020, 10, "clr",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 11, "not",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 12, "inc",0, 0, 0, 00, 1, 1, 1}
   {0x0020, 13, "dec",0, 0, 0, 00, 1, 1, 1}
   {0x0200, 10, "jmp",0, 0, 0, 00, 1, 1, 0}
   {0x0200, 11, "bne",0, 0, 0, 00, 1, 1, 0}
   {0x0200, 12, "jsr",0, 0, 0, 00, 1, 1, 0}
   {0x1000, 0, "red",0, 0, 0, 00, 1, 1, 1}
   {0x2000, 0, "prn",0, 0, 0, 01, 1, 1, 1}
   {0x4000, 0, "rts",0, 0, 0, 00, 0, 0, 0}
   {0x8000, 0, "stop",0, 0, 0, 00, 0, 0, 0}
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