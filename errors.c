
#include "data.h"
extern unsigned currentLine;

Bool yieldError(Error err)
{
    printf("\n\n######### Error!! occured on line %d:#################\n", currentLine);
    /*     illegalLabelNameUseOfCharacters

            illegalLabelNameLength

                illegalLabelNameUseOfSavedKeywords
                    illegalLabelNameUseOfCharacters */
    switch (err)
    {
    case illegalOverrideOfExternalSymbol:
        printf("Overriding of external symbol exisiting in table is not allowed!");
        break;
    case memoryAllocationFailure:
        printf("memory allocation failed");
        break;
    case undefinedOperation:
        printf("undefined operation name");
        break;
    case undefinedInstruction:
        printf("undefined instruction name");
        break;
    case tooMuchArgumentsPassed:
        printf("too Much Arguments Passed");
        break;
    case notEnoughArgumentsPassed:
        printf("too little arguments passed");
        break;
    case symbolDoesNotExist:
        printf("symbol does not exist");
        break;
    case macroDoesNotExist:
        printf("macro does not exist");
        break;
    case wrongArgumentTypePassedAsParam:
        printf("wrong Argument Type Passed As Param");
        break;
    case illegalMacroNameUseOfCharacters:
        printf("illegal Macro Name Use Of Characters");
        break;
    case illegalLabelNameUseOfCharacters:
        printf("illegal Label Name Use Of Characters");
        break;
    case illegalLabelNameLength:
        printf("illegal Label Name Length");
        break;
    case illegalMacroNameLength:
        printf("illegal Macro Name Length");
        break;
    case illegalSymbolNameAlreadyInUse:
        printf("Label Name Already In Use");
        break;
    case illegalMacroNameAlreadyInUse:
        printf("Macro Name Already In Use");
        break;
    case wrongArgumentTypeNotAnInteger:
        printf("Wrong Type, Not An Integer");
        break;
    case symbolCannotBeBothCurrentTypeAndRequestedType:
        printf("symbol Cannot Be Both Current Type And Requested Type");
        break;
    case illegalLabelUseExpectedOperationOrInstruction:
        printf("illegal Label Use Expected Operation Or Instruction");
        break;
    case expectedSingleCommaCharacter:
        printf("Missing Comma");
        break;
    case illegalApearenceOfCharactersOnLine:
        printf("Illegal use of extra characters in line");
        break;
    case wrongInstructionSyntaxExtraCommas:
        printf("Illegal use of extra commas in line");
        break;
    case expectedQuotes:
        printf("expected starting/ending quotes in string definition");
        break;

    case maxLineLengthExceeded:
        printf("line character length is illegal");

        break;
    default:
        break;
    }
    printf("\n##############################################################\n");
    printf("\n\n\n");

    return False;
}