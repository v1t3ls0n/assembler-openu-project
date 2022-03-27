
typedef enum
{
    undefinedTokenNotOperationOrInstructionOrLabel,
    illegalLabelDeclaration,
    entryDeclaredButNotDefined,
    useOfNestedMacrosIsIllegal,
    memoryAllocationFailure,
    undefinedOperation,
    undefinedInstruction,
    tooMuchArgumentsPassed,
    notEnoughArgumentsPassed,
    symbolDoesNotExist,
    macroDoesNotExist,
    wrongArgumentTypePassedAsParam,
    illegalMacroNameUseOfCharacters,
    illegalLabelNameUseOfCharacters,
    illegalLabelNameUseOfSavedKeywords,
    illegalLabelUseExpectedOperationOrInstruction,
    illegalLabelNameLength,
    illegalMacroNameLength,
    illegalSymbolNameAlreadyInUse,
    symbolCannotBeBothCurrentTypeAndRequestedType,
    illegalOverrideOfLocalSymbolWithExternalSymbol,
    labelNameAlreadyInUseInsideSymbolTable,
    illegalMacroNameAlreadyInUse,
    macroClosingWithoutAnyOpenedMacro,
    wrongArgumentTypeNotAnInteger,
    wrongInstructionSyntaxMissinCommas,
    wrongInstructionSyntaxExtraCommas,
    wrongInstructionSyntaxMissinQuotes,
    wrongOperationSyntaxMissingCommas,
    wrongOperationSyntaxExtraCommas,
    illegalApearenceOfCharactersOnLine,
    illegalApearenceOfExtraCharactersOnLine,
    illegalApearenceOfCharactersInTheEndOfTheLine,
    illegalApearenceOfCommaBeforeFirstParameter,
    illegalApearenceOfCommaAfterLastParameter,
    missinSpaceAfterInstruction,
    missingSpaceBetweenLabelDeclaretionAndInstruction,
    maxLineLengthExceeded,
    illegalOperand,
    none,
    noErrors,
    expectedNumber,
    expectedSingleCommaCharacter,
    expectedBlank,
    illegalOverrideOfExternalSymbol,
    AssemblerDidNotGetSourceFiles,
    expectedQuotes,
    closingQuotesForStringIsMissing,
    fileCouldNotBeOpened,
    wrongRegisteryReferenceUndefinedReg,
    requiredSourceOperandIsMissin,
    requiredDestinationOperandIsMissin,
    srcOperandTypeIsNotAllowed,
    desOperandTypeIsNotAllowed,
    operandTypeDoNotMatch,
    labelNotExist,
    wrongInstructionSyntaxIllegalCommaPosition,
    secondRunFailed,
    illegalMacroNameUseOfSavedKeywords,
    afterPlusOrMinusSignThereMustBeANumber,
    illegalApearenceOfCharacterInTheBegningOfTheLine,
    illegalLabelNameUseOfSavedKeywordUsingOperationName,
    illegalLabelNameUseOfSavedKeywordUsingRegisteryName,
    macroDeclaretionWithoutDefiningMacroName,
    wrongCommasSyntaxExtra,
    wrongCommasSyntaxMissing,
    wrongCommasSyntaxIllegalApearenceOfCommasInLine,
    extraOperandsPassed,
    emptyStringDeclatretion,
    emptyLabelDecleration,
    undefinedLabelDeclaretion

} Error;

typedef enum
{
    emptyDeclaretionOfEntryOrExternalVariables,

    emptyEntryDeclaretion,
    emptyExternalDeclaretion,
    instructionHasNoArguments,
    emptyDataDeclaretion

} Warning;

typedef enum
{
    firstRun,
    secondRun,
    parsingMacros,
    writeFiles,
    assemblyCodeFailedToCompile,
    exportFiles,
    goToNextFileOrEndProgram,
    stopProgram,
    endProgram,
    startProgram
} State;

typedef enum
{
    Binary,
    Hexadecimal
} EncodingFormat;
typedef enum
{
    False = 0,
    True = 1
} Bool;
