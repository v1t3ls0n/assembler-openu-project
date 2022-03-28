#include "secondRun.c"

Bool writeOperationBinary(char *operationName, char *args);
/* @ Function: writeOperationBinary
   @ Arguments: The function gets char *operationName- the name of the operation and char *args - the arguments of the operation
   @ Description: The function splits char * args to tooken- each token is a different operand.
   Then the function checks what is the kind of the operands and writes them in words at the memory.
   If there is an operand which is an undifined labl at the table of symbols, it returns false. Else it returns true.
*/

void writeAdditionalOperandsWords(Operation *op, AddrMethodsOptions active, char *value);
/* @ Function: writeAdditionalOperandsWords
   @ Arguments: The function gets Operation *op- the operation, AddrMethodsOptions active- the allowed addresing methods for the operands,
   and char * value- the operand which is about to be written in the memory.
   @ Description: The functiom checks the addressing method of the operations, then acording to the addressing method it writes value (the operand) in the memory.
    The function doesn't return value.
*/

Bool writeDataInstruction(char *token);
/* @ Function: writeDataInstruction
   @ Arguments: The function gets char *token, which is the token of the .data instruction's arguments.
   @ Description: The function spilts char *token into tokens- separate each one of the .data arguments, then it adds and writes them in the memory.
   The function returns true.
*/

Bool writeStringInstruction(char *s);
/* @ Function: writeStringInstruction
   @ Arguments: The function gets char *s, which is the string of the .string instruction.
   @ Description: The function search the opening quotes in char *s, to find out where the string starts.
   Then, it it adds and writes the characters of the string in the memory.
   The function returns true.
*/

void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], Operation *op);
