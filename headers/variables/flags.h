
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
    maxLineLengthExceeded,
    emptyLabelDecleration,
    none,
    noErrors
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
    skipLine,
    newLine,
    parseLabel,
    parseInstruction,
    parseOperation,
    printError,

    parseDataVariable,
    parseStringVariable,
    parseEntryVariable,
    parseExternalVariable,

    parseSourceOperand,
    parseDestinationOperand,

    expectNewline,
    expectNumber,
    expectComma,
    expectBlank,
    expectQuotes

} ParseState;

typedef enum
{
    False = 0,
    True = 1
} Bool;
