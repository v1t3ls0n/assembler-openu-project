
#include "data.h"
int (*line)() = &getCurrentLineNumber;
char *(*file)() = &getCurrentFileName;
Bool yieldWarning(Warning err)
{

    fprintf(stderr, "\n###################################\n");
    fprintf(stderr, "Warning!! in %s on line number %d\n", (*file)(), (*line)());
    switch (err)
    {
    case emptyLabelDecleration:
        fprintf(stderr, "empty Label Declaretion");
        break;

    case emptyDataDeclaretion:
        fprintf(stderr, "empty Data Declaretion");
        break;
    case emptyStringDeclatretion:
        fprintf(stderr, "empty String Declatretion");
        break;
    case emptyExternalDeclaretion:
        fprintf(stderr, "empty external Declatretion");
        break;

    case emptyEntryDeclaretion:
        fprintf(stderr, "empty entry Declatretion");
        break;
    case emptyDeclaretionOfEntryOrExternalVariables:
        fprintf(stderr, "empty Declaretion Of Entry Or External Variables");
        break;
    case instructionHasNoArguments:
        fprintf(stderr, "instruction Has No Arguments");
        break;

    default:
        break;
    }

    fprintf(stderr, "\n");
    fprintf(stderr, "###################################\n\n");
    return True;
}

Bool yieldError(Error err)
{
    fprintf(stderr, "\n###################################\n");
    fprintf(stderr, "Error!! occured in %s on line number %d\n", (*file)(), (*line)());

    switch (err)
    {
    case desOperandTypeIsNotAllowed:
        fprintf(stderr, "illegal input passed as destination operand!");
        break;

    case srcOperandTypeIsNotAllowed:
        fprintf(stderr, "illegal input passed as source operand!");
        break;
    case illegalOverrideOfLocalSymbolWithExternalSymbol:
        fprintf(stderr, "symbol already declared and defined locally,\nso it could not be re-declared as external variable.");
        break;
    case illegalApearenceOfCharactersInTheEndOfTheLine:
        fprintf(stderr, "illegal apearence of extra characters in the end of the line");
        break;
    case afterPlusOrMinusSignThereMustBeANumber:
        fprintf(stderr, "after Plus Or Minus Sign There Must Be A Number without any spaces between");
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
    case entryDeclaredButNotDefined:
        fprintf(stderr, "operand is registered as .entry in table but is not defined in the file");
        break;
    case requiredSourceOperandIsMissin:
    {
        fprintf(stderr, "required operand Is Missing");
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
        fprintf(stderr, "undefined registery, registeries names are r0 - r15");
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
        fprintf(stderr, "Missing Comma between argumentes");
        break;
    }

    case wrongOperationSyntaxMissingCommas:
        fprintf(stderr, "Missing Comma between operands");
        break;
    case wrongOperationSyntaxExtraCommas:
        fprintf(stderr, "Missing Comma between operands");
        break;

    case wrongInstructionSyntaxExtraCommas:
    {
        fprintf(stderr, "extra comma between arguments");
        break;
    }

    case expectedSingleCommaCharacter:
        fprintf(stderr, "Missing Comma");
        break;
    case illegalApearenceOfCharactersOnLine:
        fprintf(stderr, "Illegal use of characters");
        break;
    case undefinedTokenNotOperationOrInstructionOrLabel:
        fprintf(stderr, "illegal token in beginning of current line,  not operation nor instruction or label decleration");
        break;
    case illegalApearenceOfExtraCharactersOnLine:
        fprintf(stderr, "Illegal appearence of extra characters after the end or before begning of the line");
        break;

    case illegalApearenceOfCharacterInTheBegningOfTheLine:
        fprintf(stderr, "Illegal appearence of characters at beginning of the line");
        break;

    case expectedQuotes:
        fprintf(stderr, "expected opening quotes before string");
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