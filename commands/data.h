#include "table.h"

/* In commands.c: */
Command *getCommandByName(char *s);
Flag isLabelNameLegal(char *s);

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
/* In table.c: */
/*
extern void printSymbolTable();
extern void printSymbolItem(Item *item);
extern void setSymbolData(Item *symbol, unsigned value, Attributes attrs);
extern void setMacroData(Item *macro, char *code);
extern void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
*/
/* In verify.c: */

/* In errors.c */
extern Bool yieldError(Error err, int lineNumber);