
#include "data.h"
/* Shared global State variables*/
const char *regs[REGS_SIZE] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
unsigned currentLine = 1;
State globalState = handleMacros;
Item *symbols[HASHSIZE] = {0};
Item *macros[HASHSIZE] = {0};
