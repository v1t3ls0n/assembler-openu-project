
#include "data.h"
extern unsigned currentLineNumber;

Bool yieldError(Error err)
{
    fprintf(stderr, "\n###################################\n");
    fprintf(stderr, "Error!! occured on line %d ", currentLineNumber);

    /*     illegalLabelNameUseOfCharacters

            illegalLabelNameLength

                illegalLabelNameUseOfSavedKeywords
                    illegalLabelNameUseOfCharacters */
    switch (err)
    {

    case afterPlusOrMinusSignThereMustBeANumber:
        printf("after Plus Or Minus Sign There Must Be A Number");
        break;

    case useOfNestedMacrosIsIllegal:
    {
        fprintf(stderr, "useOfNestedMacrosIsIllegal");
        break;
    }

    case macroClosingWithoutAnyOpenedMacro:
        fprintf(stderr, "macroClosingWithoutAnyOpenedMacro");
        break;
    case missinSpaceAfterInstruction:
    {
        fprintf(stderr, "missin Space After Instruction");
        break;
    }
    case illegalApearenceOfCommaBeforeFirstParameter:
    {
        fprintf(stderr, "Illegal appearence of a comma between before the first parameter");
        break;
    }
    case illegalApearenceOfCommaAfterLastParameter:
    {
        fprintf(stderr, "Illegal appearence of a comma after the last parameter");
        break;
    }

    case wrongInstructionSyntaxIllegalCommaPosition:
    {
        fprintf(stderr, "comma appearence on line is illegal!");
        break;
    }
    case labelNotExist:
    {
        fprintf(stderr, "undefined label name, label name was not found in symbol table, assembler compilation faild, not files were created");
        break;
    }

    case illegalOperand:
    {
        fprintf(stderr, "illegal Operand!");
        break;
    }
    case operandTypeDoNotMatch:
    {
        fprintf(stderr, "Operand type does not fit to current operation");
        break;
    }

    case requiredSourceOperandIsMissin:
    {
        fprintf(stderr, "requiredSourceOperandIsMissin");
        break;
    }

    case requiredDestinationOperandIsMissin:
    {
        fprintf(stderr, "requiredDestinationOperandIsMissin");
        break;
    }
    case illegalMacroNameUseOfSavedKeywords:
        fprintf(stderr, "illegal Macro Name Use Of Saved Keywords");
        break;
    case wrongRegisteryReferenceUndefinedReg:
        fprintf(stderr, "registery name passed as operand does not exist, registeries names are r0 - r15");
        break;

    case fileCouldNotBeOpened:
        fprintf(stderr, "file could not be  opened");
        break;
    case AssemblerDidNotGetSourceFiles:
        fprintf(stderr, "You did not passed any source files to the assembler!");
        break;
    case illegalOverrideOfExternalSymbol:
        fprintf(stderr, "Overriding of external symbol exisiting in table is not allowed!");
        break;
    case memoryAllocationFailure:
        fprintf(stderr, "memory allocation failed");
        break;
    case undefinedOperation:
        fprintf(stderr, "undefined operation name");
        break;
    case undefinedInstruction:
        fprintf(stderr, "undefined instruction name");
        break;
    case tooMuchArgumentsPassed:
        fprintf(stderr, "too Much Arguments Passed");
        break;
    case notEnoughArgumentsPassed:
        fprintf(stderr, "too little arguments passed");
        break;
    case symbolDoesNotExist:
        fprintf(stderr, "symbol does not exist");
        break;
    case macroDoesNotExist:
        fprintf(stderr, "macro does not exist");
        break;
    case wrongArgumentTypePassedAsParam:
        fprintf(stderr, "wrong Argument Type Passed As Param");
        break;
    case illegalMacroNameUseOfCharacters:
        fprintf(stderr, "illegal Macro Name Use Of Characters");
        break;
    case illegalLabelNameUseOfCharacters:
        fprintf(stderr, "illegal Label Name Use Of Characters");
        break;
    case illegalLabelDeclaration:
        fprintf(stderr, "illegal Label Declaration");
        break;
    case illegalMacroNameLength:
        fprintf(stderr, "illegal Macro Name Length");
        break;
    case illegalSymbolNameAlreadyInUse:
        fprintf(stderr, "Label Name Already In Use");
        break;
    case illegalMacroNameAlreadyInUse:
        fprintf(stderr, "Macro Name Already In Use");
        break;
    case wrongArgumentTypeNotAnInteger:
        fprintf(stderr, "Number must be an integer!");
        break;
    case expectedNumber:
        fprintf(stderr, "expected number but got letter instead, argument must be an integer number");
        break;

    case symbolCannotBeBothCurrentTypeAndRequestedType:
        fprintf(stderr, "symbol Cannot Be Both Current Type And Requested Type");
        break;
    case illegalLabelUseExpectedOperationOrInstruction:
        fprintf(stderr, "illegal Label Use Expected Operation Or Instruction");
        break;
    case wrongInstructionSyntaxMissinCommas:
    {
        fprintf(stderr, "Missing Comma between operands");
        break;
    }
    case wrongInstructionSyntaxExtraCommas:
    {
        fprintf(stderr, "extra comma between operands");
        break;
    }
    case expectedSingleCommaCharacter:
        fprintf(stderr, "Missing Comma");
        break;
    case illegalApearenceOfCharactersOnLine:
        fprintf(stderr, "Illegal use of characters");
        break;
    case undefinedTokenNotOperationOrInstructionOrLabel:
        fprintf(stderr, "illegal token in begning of current line,  not operation nor instruction or label decleration");
        break;
    case illegalApearenceOfExtraCharactersOnLine:
        fprintf(stderr, "Illegal appearence of extra characters after the end or before begning of the line");
        break;

    case expectedQuotes:
        fprintf(stderr, "expected opening quotes before string decleration");
        break;

    case closingQuotesForStringIsMissing:
        fprintf(stderr, "missin closing quotes of string variable");
        break;

    case maxLineLengthExceeded:
        fprintf(stderr, "line character length is illegal");

        break;
    default:
        break;
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "###################################\n");

    return False;
}