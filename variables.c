
#include "data.h"
/* Shared global State variables*/
const char* regs[REGS_SIZE] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10",
"r11","r12","r13","r14","r15" };
unsigned currentLine = 1;
State globalState;
Item* symbols[HASHSIZE] = { 0 };
Item* macros[HASHSIZE] = { 0 };
