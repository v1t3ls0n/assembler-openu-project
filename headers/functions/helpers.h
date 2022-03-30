
/*
##########################################################################################################
##########################################################################################################
#################################### helpers.c/h General Overview: ########################################
##########################################################################################################
##########################################################################################################

this file containing some helper functions that are used in other files for doing simple and independent small
tasks like cloning a string, trimming white space, and also it contains helpers functions that are used for
encoding numeric values in binary(2's complement)/hex string.

##########################################################################################################
##########################################################################################################
##########################################################################################################
*/

/* @ Function: char *cloneString(char *s)
   @ Arguments: The function gets char *s- the string it copies
   @ Description: The function gets a char *s, if it is equal to NULL, it returns empty string.
   Else it copies the string to a new memory allocation and returns it.
*/
char *cloneString(char *s);

/* @ Function: char *trimFromLeft(char *s)
   @ Arguments: The function gets char *s- the string it trims from the white spaces from the left.
   @ Description: The function gets a char *s, while it hasn't reached the string's end,
   it skips all the white-space characters that are in the begining of this string
   and than returns a pointer to the string
   (which points to the part after the white space that were before).
*/
char *trimFromLeft(char *s);

char *decToHex(int num);
char *numToBin(int num);
HexWord *convertBinaryWordToHex(BinaryWord *word);
unsigned binaryStringToHexNumber(char binaryStr[4]);