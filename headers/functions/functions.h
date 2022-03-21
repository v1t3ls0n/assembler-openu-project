/* FUNCTIONS THAT ARE COMMENTED OUT MEANS THAT THEY ARE NOT SHARED ACROSS FILES */

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In memory.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
void writeIntoDataStack(Word *word);
void writeIntoCodeStack(Word *word);
int writeToMemory(Word *word, DataType type);
void increaseDataCounter(int amount);
void increaseInstructionCounter(int amount);
void updateFinalCountersValue();
unsigned getDC();
unsigned getIC();
unsigned getICF();
unsigned getDCF();
void printMemoryStacks(EncodingFormat format);
void initMemory();
void printBinaryImg();
void printWordBinary(unsigned index);
void wordStringToWordObj(char *s, DataType type);
void addWordToDataImage(char *s);
void addWordToCodeImage(char *s);
void addWord(int value, DataType type);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In compiler.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

int handleSourceFiles(int argc, char *argv[]);
void parseAssemblyCode(FILE *fp, char *filename, State globalState);
ParseState handleState(char *token, char *line, ParseState state, State globalState);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In preProcessor.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
void createExpandedSourceFile(FILE *source, char *fileName);
void parseMacro(FILE *fp);
FILE *createCopyFromSourceFile(FILE *source, char *fileName);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In errors.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

Bool yieldError(Error err);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In Operations.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

Operation *getOperationByName(char *s);
Operation *getOperationByIndex(unsigned int i);
int getOpIndex(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In table.c --------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void printMacroTable();
int printMacroItem(Item *item);
unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
int printSymbolItem(Item *item);
Item *getSymbol(char *name);
Item *addMacro(char *name, int start, int end);
Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Item *updateSymbolAddressValue(char *name, int newValue);
Item *getMacro(char *s);
Item *updateMacro(char *name, int start, int end);
Bool verifyLabelNaming(char *s);
Item *removeFromTable(char *name, ItemType type);
Bool verifyLabelNamingAndPrintErrors(char *s);
Bool isLabelNameAlreadyTaken(char *name, ItemType type);
void initTables();
void updateFinalMemoryAddressesInSymbolTable();
int updateSingleItemAddress(Item *item);
int getSymbolBaseAddress(char *name);
int getSymbolOffset(char *name);
Bool isExternal(char *name);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In encode.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void printObjectFile(HexWord *words[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(HexWord *words[], unsigned int ICF, unsigned int DCF);
HexWord *convertNumToHexWord(int num);
BinaryWord *convertNumberToBinaryWord(int num);
char *generateFirstWordEncodedToBinary(Operation *op);
HexWord *generateFirstWordEncodedHex(Operation *op);
Word *convertNumberToWord(int n, EncodingFormat format);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In helpers.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
unsigned binaryStringToHexNumber(char binaryStr[4]);
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
int countSpaceCharacters(char *s);
char *numToBin(int num);
int countConsecutiveCommas(char *s);
int countLengthOfNonDigitToken(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In firstRun.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
int firstRunParsing(FILE *fp, char *filename);
Bool isInstruction(char *s);
Bool isInstructionStrict(char *s);
Bool parseSingleLine(char *line, State globalState);
ParseState handleFirstToken(char *token, char *line, ParseState state);
ParseState handleOperation(char *operationName, char *args);
Bool parseOperands(char *src, char comma, char *des, Operation *op, AddrMethodsOptions active[2]);
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type);
ParseState handleInstruction(int type, char *firstToken, char *nextTokens, char *line);
ParseState handleLabel(char *labelName, char *nextToken, char *line);
Bool isLabelDeclaration(char *s);
Bool isOperation(char *s);
int getInstructionType(char *s);
char *getInstructionName(char *s);
char *getInstructionNameByType(int type);
Bool countAndVerifyStringArguments(char *tokens);
Bool isRegistery(char *s);
Bool isValidImmediateParamter(char *s);
int getRegisteryNumber(char *s);
Bool isValidIndexParameter(char *s);

Bool isComment(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In parse.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
Bool countAndVerifyDataArguments(char *line);
Bool countAndVerifyStringArguments(char *token);
char *trimFromLeft(char *s);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In Second Run.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

int secondRunParsing(FILE *fp, char *filename);
void writeDirectOperandWord(char *labelName);
void writeFirstWord(Operation *operation);
void writeSecondWord();
Bool writeOperationBinary(char *operationName, char *line);
Bool writeInstructionBinary(char *instructionName, char *line);
void parseSingleLinesecondRunParsing(char *line);
ParseState handleState(char *token, char *line, ParseState state, State globalState);
Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type);
void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], Operation *op);
void writeFirstWord(Operation *op);
void writeImmediateOperandWord(char *n);
char *parseLabelNameFromIndexAddrOperand(char *s);
int parseRegNumberFromIndexAddrOperand(char *s);
Bool writeStringInstruction(char *s);
Bool writeDataInstruction(char *s);
void writeToExternalFile(char *name, unsigned base, unsigned offset);