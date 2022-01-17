#include "instructions.h"

int main()
{
    printf("\nSource Addresing Methods:\n0:%d 1:%d 2:%d 3:%d", instructionsTable[0].src.immediate, instructionsTable[0].src.direct, instructionsTable[0].src.index, instructionsTable[0].src.reg);
    printf("\nDestination Addresing Methods:\n0:%d 1:%d 2:%d 3:%d", instructionsTable[0].des.immediate, instructionsTable[0].des.direct, instructionsTable[0].des.index, instructionsTable[0].des.reg);

    return 0;
}