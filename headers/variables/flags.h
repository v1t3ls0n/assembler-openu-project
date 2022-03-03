
typedef enum
{
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
    wrongArgumentTypeNotAnInteger,
    wrongInstructionSyntaxMissinCommas,
    wrongInstructionSyntaxExtraCommas,
    wrongInstructionSyntaxMissinQuotes,
    illegalApearenceOfCharactersOnLine,
    illegalApearenceOfExtraCharactersOnLine,
    maxLineLengthExceeded,
    emptyLabelDecleration,
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
    operandTypeDoNotMatch
} Error;

typedef enum
{
    firstRun,
    secondRun,
    handleMacros,
    writeFiles,
    collectErrors,
    stopProgram,
    endProgram,
    startProgram
} State;

typedef enum
{
    skipLine = 134,
    newLine = 233,
    parseLabel = 4364,
    parseInstruction = 55,
    parseOperation = 333,
    returnFalse = 4678,
    lineParsedSuccessfully = 23,
    parsingLine = 112,
    parsingMacro = 86,
    replacingMacro = 11,
    Err = 0,
    parseDataVariables = 3,
    parseStringVariables = 98,
    parseEntryVariable = 97,
    parseExternalVariable = 953,
    parseSourceOperand = 77,
    parseDestinationOperand = 93,
    expectNewline = 92
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
