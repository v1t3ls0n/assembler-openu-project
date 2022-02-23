
typedef enum
{
    memoryAllocationFailure,
    undefinedCommand,
    undefinedInstruction,
    tooMuchArgumentsPassed,
    notEnoughArgumentsPassed,
    symbolDoesNotExist,
    macroDoesNotExist,
    wrongArgumentTypePassedAsParam,
    illegalMacroNameUseOfCharacters,
    illegalLabelNameUseOfCharacters,
    illegalLabelNameUseOfSavedKeywords,
    illegalLabelUseExpectedCommandOrInstruction,
    illegalLabelNameLength,
    illegalMacroNameLength,
    illegalSymbolNameAlreadyInUse,
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
    parseCommand,
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
