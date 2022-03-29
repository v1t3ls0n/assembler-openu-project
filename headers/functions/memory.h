
/*
##########################################################################################################
##########################################################################################################
#################################### memory.c/h General Overview: ########################################
##########################################################################################################
##########################################################################################################

This memory purpose is for managing the memory of the resulted binary/hex image, it is used in the first
run and the second run as well. in the first run functions in here used to increase and decrease the DC/IC
counters and in the second run the function included here builds the memory image word by word.

##########################################################################################################
##########################################################################################################
##########################################################################################################
*/

void increaseDataCounter(int amount);
/* @ Function: increaseDataCounter
   @ Arguments: The function gets int amount- the amount of addresses in the memory the data counter holds.
   @ Description: The function increases the data counter by amount- the amount of addresses in the memory the data counter holds.
   The function doesn't return any value.
*/

void increaseInstructionCounter(int amount);
/* @ Function: increaseInstructionCounter
   @ Arguments: The function gets int amount- the amount of addresses in the memory the instruction counter holds.
   @ Description: The function increases the instruction counter by amount- the amount of addresses in the memory the instruction counter holds.
   The function does'nt return any value.
*/

void allocMemoryImg();
/* @ Function: allocMemoryImg
   @ Arguments: The function gets no arguments.
   @ Description: The function allocates the exact amount of space required to generate the compiled image.
   The function doesn't return any value.
*/
void resetMemoryCounters();
/* @ Function: resetMemoryCounters
   @ Arguments: The function gets no arguments.
   @ Description: The function reset the counter that counts the exact amount of space required to generate the compiled image.
   The function doesn't return any value.
*/

void wordStringToWordObj(char *s, DataType type);
/* @ Function: wordStringToWordObj
   @ Arguments: The function gets char *s - the string it converts and DataType type- tells us if the current word belongs to the instructions or to the tha data part of the memory image.
   converts a string from a binary word to typedef struct
   @ Description: The function converts a string to a binaryWord typedef struct (defines in complex_typedef.h).
   The function doesn't return any value.
*/

void writeMemoryImageToObFile(FILE *fp);
/* @ Function: writeMemoryImageToObFile
   @ Arguments: The function gets FILE *fp the file it writes the ob memory into
   @ Description: The function generates the .ob memory image of the code. It writes into the .ob file word by word using helper functions to convert binary word into an hex word.
   The function doesn't return any value.
*/

void printMemoryImgInRequiredObjFileFormat();
void calcFinalAddrsCountersValues();
void printWordBinary(unsigned index);
void addWordToCodeImage(char *s);
void addWordToDataImage(char *s);
void addWord(int value, DataType type);
void printBinaryImg();

unsigned getDC();
unsigned getIC();
unsigned getICF();
unsigned getDCF();