#include "table.h"

/* In commands.c: */
Command *getCommandByName(char *s);
void verifyLabelNaming(char *s);

/* In encode.c: */
extern void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
extern void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
extern EncodedWord *encodeIntNum(int num);
extern char *generateFirstWordEncodedToBinary(Command *cmd);
extern EncodedWord *generateFirstWordEncodedHex(Command *cmd);

/* In helpers.c: */
extern char *decToHex(int num);
extern char *hexToBin(char *hex);
extern int hex2int(char ch);
extern char *dec2Bin2sComplement(int n);

/* In table.c: */

extern void printSymbolTable();
extern void printSymbolItem(Item *item);
extern void setSymbolData(Item *symbol, unsigned value, Attributes attrs);
extern void setMacroData(Item *macro, char *code);
extern Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);

/* In verify.c: */

/* In errors.c */
extern void yieldError(Error err, int lineNumber);

/*
extern void parseSingleLine(char *line);
 */
/* in memory.c */
extern int writeToMemory(EncodedWord value, ItemType type);
extern void updateSymbolTableFinalValues();
extern void updateDataEntry(Item *p);