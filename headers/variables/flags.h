
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
    illegalInputPassedAsOperandSrcOperand,
    illegalInputPassedAsOperandDesOperand,
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
    undefinedLabelDeclaretion,
    registeryIndexOperandTypeIfOutOfAllowedRegisteriesRange

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
    assemblyCodeFailedToCompile,
    startProgram,
    parsingMacros,
    firstRun,
    secondRun,
    exportFiles,
    exitProgram
} State;

typedef enum
{
    False = 0,
    True = 1
} Bool;