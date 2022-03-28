#include "./headers/lib/lib.h"
#include "./headers/variables/variables.h"

char *cloneString(char *s);
char *trimFromLeft(char *s);
char *decToHex(int num);
char *numToBin(int num);
HexWord *convertBinaryWordToHex(BinaryWord *word);
unsigned binaryStringToHexNumber(char binaryStr[4]);