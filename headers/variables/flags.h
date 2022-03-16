
typedef enum
{
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
    wrongArgumentTypeNotAnInteger,
    wrongInstructionSyntaxMissinCommas,
    wrongInstructionSyntaxExtraCommas,
    wrongInstructionSyntaxMissinQuotes,
    illegalApearenceOfCharactersOnLine,
    illegalApearenceOfExtraCharactersOnLine,
    illegalApearenceOfCommaBeforeFirstParameter,
    illegalApearenceOfCommaAfterLastParameter,
    missinSpaceAfterInstruction,
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
    operandTypeDoNotMatch,
    labelNotExist,
    wrongInstructionSyntaxIllegalCommaPosition,
    secondRunFailed,
    illegalMacroNameUseOfSavedKeywords
} Error;

typedef enum
{
    firstRun,
    secondRun,
    replacingMacros,
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
    skipToNextToken = 359,
    parseLabel = 4364,
    parseInstruction = 55,
    parseOperation = 333,
    returnFalse = 4678,
    lineParsedSuccessfully = 11111111,
    parsingLine = 112,
    parsingMacro = 86,
    replacingMacro = 11,
    Err = 0,
    parseDataVariables = 3,
    parseMacroName,
    parseMacroContent,
    parseStringVariables = 98,
    parseEntryVariable = 97,
    parseExternalVariable = 953,
    parseSourceOperand = 77,
    parseDestinationOperand = 93,
    expectNewline = 92,
    writingOperationIntoMemoryImg,
    writingDataIntoMemoryImg,
    genFirstWord,
    genSecondWord,
    endParsingOfMacroThatHaveNoEndingTag,
    evalToken

} ParseState;
/* typedef enum
{
    skipLine = 134,
    newLine = 233,
    parseLabel = 4364,
    parseInstruction = 55,
    parseOperation = 333,
    Err = 0,
    parseDataVariables = 3,
    parseStringVariables = 98,
    parseEntryVariable = 97,
    parseExternalVariable = 953,
    parseSourceOperand = 77,
    parseDestinationOperand = 93,
    expectNewline = 92,
    writingOperationIntoMemoryImg,
    writingDataIntoMemoryImg,
    genFirstWord,
    genSecondWord,
} ParseState; */
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
