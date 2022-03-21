
typedef enum
{
    undefinedTokenNotOperationOrInstructionOrLabel,
    illegalLabelDeclaration,
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
    illegalApearenceOfCharacterInTheBegningOfTheLine

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
    collectErrors,
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
    skipLine = 2,
    newLine = 3,
    skipToNextToken = 4,
    parseLabel = 5,
    parseInstruction = 6,
    parseOperation = 7,
    parsingLine = 8,

    parsingMacro,
    parsingMacroName = 9,
    parsingMacroContent = 10,
    endParsingOfMacroThatHaveNoEndingTag = 11,
    evalToken = 12,
    parseDataVariables = 13,
    parseStringVariables = 14,
    parseEntryVariable = 15,
    parseExternalVariable = 16,
    parseSourceOperand = 17,
    parseDestinationOperand = 18,
    writingOperationIntoMemoryImg = 19,
    writingDataIntoMemoryImg = 20

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
