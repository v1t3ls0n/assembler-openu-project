
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
    illegalLabelNameUseOfSavedKeywordUsingRegisteryName

} Error;

typedef enum
{
    emptyDeclaretionOfEntryOrExternalVariables,
    emptyStringDeclatretion,
    emptyEntryDeclaretion,
    emptyExternalDeclaretion,
    instructionHasNoArguments,
    emptyDataDeclaretion,
    emptyLabelDecleration

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
    Err = 0,
    lineParsedSuccessfully = 1,
    newLine = 3,
    skipToNextToken = 4,
    parseLabel = 5,
    parseInstruction = 6,
    parseOperation = 7,
    parsingLine = 8,
    parseDataVariables = 13,
    parseStringVariables = 14,
    parseEntryVariable = 15,
    parseExternalVariable = 16,
    parseSourceOperand = 17,
    parseDestinationOperand = 18,
    writingOperationIntoMemoryImg = 19,
    writingDataIntoMemoryImg = 20,

    skipLine,
    writeToTargetFile,
    evalToken,
    readToken,
    saveCharToToken,
    deleteMacroFromTarget,
    deleteLastLine,
    parsingMacroName,
    searchingMacroStart,
    searchingMacroEnd,
    addMacroToTable,
    replaceMacro,
    parsingMacroContent,
    parsingToken,
    replacingMacro

} ParseState;

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
