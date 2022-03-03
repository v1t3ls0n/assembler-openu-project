
#include "data.h"
extern unsigned currentLine;

Bool yieldError(Error err)
{
    printf("\n###################################\n");
    printf("Error!! occured on line %d ", currentLine);
    /*     illegalLabelNameUseOfCharacters

            illegalLabelNameLength

                illegalLabelNameUseOfSavedKeywords
                    illegalLabelNameUseOfCharacters */
    switch (err)
    {
    case wrongRegisteryReferenceUndefinedReg:
        printf("registery name passed as operand does not exist, registeries names are r0 - r15");
        break;

    case fileCouldNotBeOpened:
        printf("file could not be  opened");
        break;
    case AssemblerDidNotGetSourceFiles:
        printf("You did not passed any source files to the assembler!");
        break;
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
        printf("Number must be an integer!");
        break;
    case expectedNumber:
        printf("Only integer numbers allowed!");
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
        printf("Illegal use of characters");
        break;
    case wrongInstructionSyntaxExtraCommas:
        printf("Illegal use of extra commas in line");
        break;
    case expectedQuotes:
        printf("expected opening quotes before string decleration");
        break;

    case closingQuotesForStringIsMissing:
        printf("missin closing quotes of string variable");
        break;

    case maxLineLengthExceeded:
        printf("line character length is illegal");

        break;
    default:
        break;
    }
    printf("\n");
    printf("###################################\n");

    return False;
}