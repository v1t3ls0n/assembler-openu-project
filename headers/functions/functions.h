/* FUNCTIONS THAT ARE COMMENTED OUT MEANS THAT THEY ARE NOT SHARED ACROSS FILES */
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In errors.c: ----------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void yieldError(Error err, int lineNumber);

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
Item *findOrAddSymbol(char *name, ItemType type);
Item *findSymbol(char *name, ItemType type);
Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
void updateSymbol(char *name, int newValue);
char *getMacroCodeValue(char *s);
void addMacro(char *name, char *code);
void verifyLabelNaming(char *s);
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In encode.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
EncodedWord *encodeIntNum(int num);
char *generateFirstWordEncodedToBinary(Operation *op);
EncodedWord *generateFirstWordEncodedHex(Operation *op);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In helpers.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
EncodedWord *dec2Bin2sComplement(int n);

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In parse.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
int parseSingleLine(char *line, int lineNumber);
int handleState(char *token, ParseState state);
int handleOperation(Operation *op, char *operands, char *line);
int handleInstruction(int type, char *labelName, char *nextTokens);
int handleLabel(char *labelName, char *nextToken, char *line);
int isLabel(char *s);
int isOperation(char *s);
int isInstruction(char *s);
char *getInstructionName(char *s);
int calcLineMemoryUsage(Operation *op, char *srcOperand, char *desOperand);
void updateMemoryCounters();
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------In memory.c -------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
void writeIntoDataStack(Word *word);
int writeToMemory(Word *word, DataType type);
int writeToMemory(Word *word, DataType type);
void updateSymbolTableFinalValues();
void updateDataEntry(Item *p);
void increaseDataCounter(int amount);
void inceaseInstructionCounter(int amount);
void resetCounters();
