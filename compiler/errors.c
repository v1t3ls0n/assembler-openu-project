#include <stdio.h>
#include "flags.h"
void yieldError(Error err, int lineNumber)
{
    printf("Error!! occured on line %d:\n", lineNumber);

    switch (err)
    {
    case memoryAllocationFailure:
        printf("memory allocation failed");
        break;
    case undefinedCommand:
        printf("undefined command name");
        break;
    case undefinedInstruction:
        break;
    case tooMuchArgumentsPassed:
        break;
    case notEnoughArgumentsPassed:
        break;
    case symbolDoesNotExist:
        break;
    case macroDoesNotExist:
        break;
    case wrongArgumentTypePassedAsParam:
        break;
    case illegalMacroNameUseOfCharacters:
        break;
    case illegalLabelNameUseOfCharacters:
        break;
    case illegalLabelNameLength:
        break;
    case illegalMacroNameLength:
        break;
    case illegalSymbolNameAlreadyInUse:
        break;
    case illegalMacroNameAlreadyInUse:
        break;
    case wrongArgumentTypeNotAnInteger:
        break;
    case wrongInstructionSyntaxMissinCommas:
        break;
    case wrongInstructionSyntaxExtraCommas:
        break;
    case wrongInstructionSyntaxMissinQuotes:
        break;
    case illegalApearenceOfCharactersOnLine:
        break;
    case maxLineLengthExceeded:
        break;
    default:
        break;
    }
    printf("\n");
}