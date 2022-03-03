/* FUNCTIONS THAT ARE COMMENTED OUT MEANS THAT THEY ARE NOT SHARED ACROSS FILES */
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In compiler.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

int handleSourceFiles(int argc, char *argv[]);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In preProcessor.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void parseSourceFile(FILE *source, char *filename);
void replaceWithMacro(FILE *p, int startIndex, int endIndex);
void parseMacro(FILE *fp);
int readFromFileByIndexes(FILE *fptr, char *filename, int start, int end);
FILE *createCopyFromSourceFile(FILE *source, char *fileName);
int parseNextLine(char *line, int start, int end);
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

unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
void printSymbolItem(Item *item);
Item *getSymbol(char *name, ItemType type);
Bool addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Bool updateSymbol(Item *p, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Item *updateSymbolAddressValue(char *name, int newValue);
Item *getMacro(char *s);
Item *addMacro(char *name, int start, int end);
Bool verifyLabelNaming(char *s);
Item *removeFromTable(char *name, ItemType type);

Bool isLabelNameAlreadyTaken(char *name, ItemType type);
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
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
unsigned char dec2Bin2sComplement(int n);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In parse.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
int parseExpandedSourceFile(FILE *fp, char *filename);
Bool isInstruction(char *s);
int parseSingleLine(char *line, ParseState state);
int handleState(char *token, char *line, ParseState state);
int handleOperation(char *op, char *operands);
int handleInstruction(int type, char *firstToken, char *nextTokens);
int handleLabel(char *labelName, char *nextToken, char *line);
int isLabel(char *s);
int isOperation(char *s);
int getInstructionType(char *s);
char *getInstructionName(char *s);
char *getInstructionNameByType(int type);
int handleInstructionDataArgs(char *tokens);
int handleInstructionStringArgs(char *tokens);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In memory.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
void writeIntoDataStack(Word *word);
void writeIntoCodeStack(Word *word);
int writeToMemory(Word *word, DataType type);
void updateSymbolTableFinalValues();
void updateDataEntry(Item *p);
void increaseDataCounter(int amount);
void increaseInstructionCounter(int amount);
void resetCounters();
unsigned getDC();
unsigned getIC();
void printMemoryStacks(EncodingFormat format);
void addNumberToMemory(int number);