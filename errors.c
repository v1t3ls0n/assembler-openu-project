#include "data.h"

static int (*line)() = &getCurrentLineNumber;
static char *(*filePath)() = &getFileNamePath;
static FILE *warningsFile = NULL, *errorsFile = NULL;
static Bool isWarningFileExist = False;
static Bool isErrorFileExist = False;

void fileCreationFailure(char *fileName)
{
    extern FILE *errorsFile;
    extern Bool isErrorFileExist;

    if (!isErrorFileExist)
    {
        if ((errorsFile = fopen("errors.log", "w+")) == NULL)
        {
            fprintf(stderr, "\n######################################################################\n");
            fprintf(stderr, " FAILURE! failed to create %s error log file\n", fileName);
            fprintf(stderr, "######################################################################\n\n");
        }
        else
            isErrorFileExist = True;
    }

    fprintf(stderr, "\n######################################################################\n");
    fprintf(stderr, " FAILURE! failed to create %s file\n", fileName);
    fprintf(stderr, "######################################################################\n\n");

    fprintf(errorsFile, "\n######################################################################\n");
    fprintf(errorsFile, " FAILURE! failed to create %s file\n", fileName);
    fprintf(errorsFile, "######################################################################\n\n");
}

void fileOpeningFailure(char *fileName)
{
    extern FILE *errorsFile;
    extern Bool isErrorFileExist;

    if (!isErrorFileExist)
    {
        if ((errorsFile = fopen("errors.log", "w+")) == NULL)
        {
            fprintf(stderr, "\n######################################################################\n");
            fprintf(stderr, " FAILURE! failed to create %s error log file\n", fileName);
            fprintf(stderr, "######################################################################\n\n");
        }
        else
            isErrorFileExist = True;
    }

    fprintf(stderr, "\n######################################################################\n");
    fprintf(stderr, " FAILURE! failed to open %s file\n", fileName);
    fprintf(stderr, "######################################################################\n\n");

    fprintf(errorsFile, "\n######################################################################\n");
    fprintf(errorsFile, " FAILURE! failed to open %s file\n", fileName);
    fprintf(errorsFile, "######################################################################\n\n");
}

void yieldWarningIntoFile(Warning err)
{
    extern Bool isWarningFileExist;
    extern FILE *warningsFile;
    if (!isWarningFileExist)
    {

        if ((warningsFile = fopen("warnings.log", "w+")) == NULL)
            printf("Failed to open warning log file\n");
        else
            isWarningFileExist = True;
    }

    fprintf(warningsFile, "\n######################################################################\n");
    fprintf(warningsFile, "Warning!! in %s on line number %d\n", (*filePath)(), (*line)());
    switch (err)
    {

    case emptyDataDeclaretion:
        fprintf(warningsFile, "empty Data Declaretion");
        break;

    case emptyExternalDeclaretion:
        fprintf(warningsFile, "empty external Declatretion");
        break;

    case emptyEntryDeclaretion:
        fprintf(warningsFile, "empty entry Declatretion");
        break;
    case emptyDeclaretionOfEntryOrExternalVariables:
        fprintf(warningsFile, "empty Declaretion Of Entry Or External Variables");
        break;
    case instructionHasNoArguments:
        fprintf(warningsFile, "instruction Has No Arguments");
        break;

    default:
        break;
    }

    fprintf(warningsFile, "\n");
    fprintf(warningsFile, "######################################################################\n\n");
}

void yieldErrorIntoFile(Error err)
{
    extern FILE *errorsFile;
    extern Bool isErrorFileExist;
    if (!isErrorFileExist)
    {
        if ((errorsFile = fopen("errors.log", "w+")) == NULL)
            printf("Failed to open erro log file\n");
        else
            isErrorFileExist = True;
    }
    fprintf(errorsFile, "\n######################################################################\n");
    fprintf(errorsFile, "Error!! occured in %s on line number %d\n", (*filePath)(), (*line)());

    switch (err)
    {
    case macroDeclaretionWithoutDefiningMacroName:
        fprintf(errorsFile, "Macro declaration without defining macro name!");
        break;

    case registeryIndexOperandTypeIfOutOfAllowedRegisteriesRange:
        fprintf(errorsFile, "illegal use of registery index operand addressing method\nRegistery Index addr method is allows to use only registeries\n that are between the r10 - r15!");
        break;
    case illegalInputPassedAsOperandSrcOperand:
        fprintf(errorsFile, "illegal operands input passed to source operand ");
        break;

    case illegalInputPassedAsOperandDesOperand:
        fprintf(errorsFile, "illegal operands input passed to destination operand ");
        break;
    case undefinedLabelDeclaretion:
        fprintf(errorsFile, "undefined Label Declaretion");
        break;
    case emptyLabelDecleration:
        fprintf(errorsFile, "empty Label Declaretion");
        break;
    case emptyStringDeclatretion:
        fprintf(errorsFile, "empty String Declatretion");
        break;
    case missingSpaceBetweenLabelDeclaretionAndInstruction:
        fprintf(errorsFile, "Missing space between label declaretion and instruction name");
        break;
    case extraOperandsPassed:
        fprintf(errorsFile, "Extra operands passed as paramters");
        break;
    case wrongCommasSyntaxIllegalApearenceOfCommasInLine:
        fprintf(errorsFile, "Illegal apearence of commas in line");
        break;
    case wrongCommasSyntaxExtra:
        fprintf(errorsFile, "Extra commas between arguments");
        break;

    case wrongCommasSyntaxMissing:
        fprintf(errorsFile, "Missing Commas between arguments");
        break;
    case illegalLabelNameLength:
        fprintf(errorsFile, "illegal Label Name length is greater than the maximum allowed which is %d characters", MAX_LABEL_LEN);
        break;
    case illegalLabelNameUseOfSavedKeywordUsingOperationName:
        fprintf(errorsFile, "illegal Label Name Use Of Saved Keyword.\nUsing Operation Name is not allowed");
        break;

    case illegalLabelNameUseOfSavedKeywordUsingRegisteryName:
        fprintf(errorsFile, "illegal Label Name Use Of Saved Keyword.\nUsing Registery Name is not allowed");
        break;

    case illegalLabelNameUseOfSavedKeywords:
        fprintf(errorsFile, "illegal Label Name Use Of Saved Keyword.\n");
        break;
    case desOperandTypeIsNotAllowed:
        fprintf(errorsFile, "type of destination operand passed to operation is not accepted!");
        break;

    case srcOperandTypeIsNotAllowed:
        fprintf(errorsFile, "type of source operand passed to operation is not accepted!");
        break;
    case illegalOverrideOfLocalSymbolWithExternalSymbol:
        fprintf(errorsFile, "symbol already declared and defined locally,\nso it could not be re-declared as external variable.");
        break;
    case illegalApearenceOfCharactersInTheEndOfTheLine:
        fprintf(errorsFile, "illegal apearence of extra characters in the end of the line");
        break;
    case afterPlusOrMinusSignThereMustBeANumber:
        fprintf(errorsFile, "after Plus Or Minus Sign There Must Be A Number without any spaces between");
        break;

    case useOfNestedMacrosIsIllegal:
    {
        fprintf(errorsFile, "useOfNestedMacrosIsIllegal");
        break;
    }

    case macroClosingWithoutAnyOpenedMacro:
        fprintf(errorsFile, "macroClosingWithoutAnyOpenedMacro");
        break;
    case missinSpaceAfterInstruction:
    {
        fprintf(errorsFile, "missin Space between instruction and arguments");
        break;
    }
    case illegalApearenceOfCommaBeforeFirstParameter:
    {
        fprintf(errorsFile, "Illegal appearence of a comma before the first parameter");
        break;
    }
    case illegalApearenceOfCommaAfterLastParameter:
    {
        fprintf(errorsFile, "Illegal appearence of a comma after the last parameter");
        break;
    }

    case wrongInstructionSyntaxIllegalCommaPosition:
    {
        fprintf(errorsFile, "comma appearence on line is illegal!");
        break;
    }
    case labelNotExist:
    {
        fprintf(errorsFile, "undefined label name, label name was not found in symbol table, assembler compilation faild, not files were created");
        break;
    }

    case operandTypeDoNotMatch:
    {
        fprintf(errorsFile, "Operand type does not fit to current operation");
        break;
    }
    case entryDeclaredButNotDefined:
        fprintf(errorsFile, "operand is registered as .entry in table but is not defined in the file");
        break;
    case requiredSourceOperandIsMissin:
    {
        fprintf(errorsFile, "required source operand is missing");
        break;
    }

    case requiredDestinationOperandIsMissin:
    {
        fprintf(errorsFile, "required destination operand is missing");
        break;
    }
    case illegalMacroNameUseOfSavedKeywords:
        fprintf(errorsFile, "illegal Macro Name Use Of Saved Keywords");
        break;
    case wrongRegisteryReferenceUndefinedReg:
        fprintf(errorsFile, "undefined registery, registeries names are r0 - r15");
        break;

    case fileCouldNotBeOpened:
        fprintf(errorsFile, "file could not be  opened");
        break;

    case illegalOverrideOfExternalSymbol:
        fprintf(errorsFile, "Overriding of external symbol exisiting in table is not allowed!");
        break;
    case memoryAllocationFailure:
        fprintf(errorsFile, "memory allocation failed");
        break;
    case undefinedOperation:
        fprintf(errorsFile, "undefined operation name");
        break;
    case undefinedInstruction:
        fprintf(errorsFile, "undefined instruction name");
        break;
    case tooMuchArgumentsPassed:
        fprintf(errorsFile, "too Much Arguments Passed");
        break;
    case notEnoughArgumentsPassed:
        fprintf(errorsFile, "too little arguments passed");
        break;
    case symbolDoesNotExist:
        fprintf(errorsFile, "symbol does not exist");
        break;
    case macroDoesNotExist:
        fprintf(errorsFile, "macro does not exist");
        break;
    case wrongArgumentTypePassedAsParam:
        fprintf(errorsFile, "wrong Argument Type Passed As Param");
        break;
    case illegalMacroNameUseOfCharacters:
        fprintf(errorsFile, "illegal Macro Name Use Of Characters");
        break;
    case illegalLabelNameUseOfCharacters:
        fprintf(errorsFile, "illegal Label Name Use Of Characters");
        break;

    case illegalLabelDeclaration:
        fprintf(errorsFile, "illegal Label Declaration");
        break;
    case illegalMacroNameLength:
        fprintf(errorsFile, "illegal Macro Name Length");
        break;
    case illegalSymbolNameAlreadyInUse:
        fprintf(errorsFile, "Label Name Already In Use");
        break;
    case illegalMacroNameAlreadyInUse:
        fprintf(errorsFile, "Macro Name Already In Use");
        break;
    case wrongArgumentTypeNotAnInteger:
        fprintf(errorsFile, "Number must be an integer!");
        break;
    case expectedNumber:
        fprintf(errorsFile, "expected number but got letter instead, argument must be an integer number");
        break;

    case symbolCannotBeBothCurrentTypeAndRequestedType:
        fprintf(errorsFile, "symbol Cannot Be Both Current Type And Requested Type");
        break;
    case illegalLabelUseExpectedOperationOrInstruction:
        fprintf(errorsFile, "illegal Label Use Expected Operation Or Instruction");
        break;
    case wrongInstructionSyntaxMissinCommas:
    {
        fprintf(errorsFile, "Missing Comma between argumentes");
        break;
    }

    case wrongOperationSyntaxMissingCommas:
        fprintf(errorsFile, "Missing Comma between operands");
        break;
    case wrongOperationSyntaxExtraCommas:
        fprintf(errorsFile, "Extra Comma between operands");
        break;

    case wrongInstructionSyntaxExtraCommas:
    {
        fprintf(errorsFile, "extra comma between arguments");
        break;
    }

    case expectedSingleCommaCharacter:
        fprintf(errorsFile, "Missing Comma");
        break;
    case illegalApearenceOfCharactersOnLine:
        fprintf(errorsFile, "Illegal use of characters");
        break;
    case undefinedTokenNotOperationOrInstructionOrLabel:
        fprintf(errorsFile, "illegal token in beginning of current line,  not operation nor instruction or label decleration");
        break;
    case illegalApearenceOfExtraCharactersOnLine:
        fprintf(errorsFile, "Illegal appearence of extra characters after the end or before begning of the line");
        break;

    case illegalApearenceOfCharacterInTheBegningOfTheLine:
        fprintf(errorsFile, "Illegal appearence of characters at beginning of the line");
        break;

    case expectedQuotes:
        fprintf(errorsFile, "expected opening quotes before string");
        break;

    case closingQuotesForStringIsMissing:
        fprintf(errorsFile, "missin closing quotes of string variable");
        break;

    case maxLineLengthExceeded:
        fprintf(errorsFile, "line character length is illegal");

        break;
    default:
        break;
    }
    fprintf(errorsFile, "\n");
    fprintf(errorsFile, "######################################################################\n");
}
Bool yieldWarning(Warning err)
{
    yieldWarningIntoFile(err);
    fprintf(stderr, "\n######################################################################\n");
    fprintf(stderr, "Warning!! in %s on line number %d\n", (*filePath)(), (*line)());
    switch (err)
    {

    case emptyDataDeclaretion:
        fprintf(stderr, "empty Data Declaretion");
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
    fprintf(stderr, "######################################################################\n\n");
    return True;
}

Bool yieldError(Error err)
{
    yieldErrorIntoFile(err);
    fprintf(stderr, "\n######################################################################\n");
    fprintf(stderr, "Error!! occured in %s on line number %d\n", (*filePath)(), (*line)());

    switch (err)
    {

    case macroDeclaretionWithoutDefiningMacroName:
        fprintf(stderr, "Macro declaration without defining macro name!");
        break;
    case registeryIndexOperandTypeIfOutOfAllowedRegisteriesRange:
        fprintf(stderr, "illegal use of registery index operand addressing method\nRegistery Index addr method is allows to use only registeries\n that are between the r10 - r15!");
        break;
    case illegalInputPassedAsOperandSrcOperand:
        fprintf(stderr, "illegal operands input passed to source operand ");
        break;

    case illegalInputPassedAsOperandDesOperand:
        fprintf(stderr, "illegal operands input passed to destination operand ");
        break;
    case desOperandTypeIsNotAllowed:
        fprintf(stderr, "type of destination operand passed to operation is not accepted!");
        break;

    case srcOperandTypeIsNotAllowed:
        fprintf(stderr, "type of source operand passed to operation is not accepted!");
        break;
    case undefinedLabelDeclaretion:
        fprintf(stderr, "undefined Label Declaretion");
        break;
    case emptyLabelDecleration:
        fprintf(stderr, "empty Label Declaretion");
        break;
    case emptyStringDeclatretion:
        fprintf(stderr, "empty String Declatretion");
        break;
    case missingSpaceBetweenLabelDeclaretionAndInstruction:
        fprintf(stderr, "Missing space between label declaretion and instruction name");
        break;
    case illegalLabelNameLength:
        fprintf(stderr, "illegal Label Name length is greater than the maximum allowed which is %d characters", MAX_LABEL_LEN);
        break;
    case extraOperandsPassed:
        fprintf(stderr, "Extra operands passed as paramters");
        break;

    case wrongCommasSyntaxIllegalApearenceOfCommasInLine:
        fprintf(stderr, "Illegal apearence of commas in line");
        break;
    case wrongCommasSyntaxExtra:
        fprintf(stderr, "Extra commas between arguments");
        break;

    case wrongCommasSyntaxMissing:
        fprintf(stderr, "Missing Commas between arguments");
        break;

    case illegalLabelNameUseOfSavedKeywordUsingOperationName:
        fprintf(stderr, "illegal Label Name Use Of Saved Keyword.\nUsing Operation Name is not allowed");
        break;

    case illegalLabelNameUseOfSavedKeywordUsingRegisteryName:
        fprintf(stderr, "illegal Label Name Use Of Saved Keyword.\nUsing Registery Name is not allowed");
        break;

    case illegalLabelNameUseOfSavedKeywords:
        fprintf(stderr, "illegal Label Name Use Of Saved Keyword.\n");
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
        fprintf(stderr, "missin Space between instruction and arguments");
        break;
    }
    case illegalApearenceOfCommaBeforeFirstParameter:
    {
        fprintf(stderr, "Illegal appearence of a comma before the first parameter");
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
        fprintf(stderr, "required source operand is missing");
        break;
    }

    case requiredDestinationOperandIsMissin:
    {
        fprintf(stderr, "required destination operand is missing");
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
        fprintf(stderr, "Extra Comma between operands");
        break;

    case wrongInstructionSyntaxExtraCommas:
    {
        fprintf(stderr, "Extra comma between arguments");
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
    fprintf(stderr, "######################################################################\n");

    return False;
}

void closeOpenLogFiles()
{
    extern FILE *warningsFile, *errorsFile;
    extern Bool isWarningFileExist, isErrorFileExist;
    if (isWarningFileExist && warningsFile != NULL)
    {
        fclose(warningsFile);
        isWarningFileExist = False;
    }
    if (isErrorFileExist && errorsFile != NULL)
    {
        fclose(errorsFile);
        isErrorFileExist = False;
    }
}