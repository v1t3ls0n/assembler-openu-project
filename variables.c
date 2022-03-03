
#include "data.h"
/* Shared global State variables*/

unsigned currentLine = 1;
State globalState = handleMacros;
Item *symbols[HASHSIZE] = {0};
Item *macros[HASHSIZE] = {0};
