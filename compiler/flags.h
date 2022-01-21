
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
    noErrors
} Error;

typedef enum
{
    ignoreBlanks,
    newLine,
    parseToken,
    skipLine,
    insideCommentLine,
    parseDataArguments,
    parseStringArguments,
    parseCommandArguments,
    inComment,
    expectNewline,
    expectNumber,
    expectComma,
    expectBlank,
    expectStartingDoubleQuote,
    expectEndingDoubleQuote
} ParseMode;

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
    False,
    True
} Bool;
