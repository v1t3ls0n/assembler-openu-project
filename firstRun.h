#include "firstRun.c"

Bool handleOperation(char *operationName, char *args);

/* @ Function: handleOperation
   @ Arguments: The function gets char *operationName which the name of the current operation and char * args which are the arguments of the operation.
   @ Description:This function is the main function of parsing operations. It splits the operands from the line, it calls to the functions to validate them.
   For instance, it calls to parseOperand to validate that all of the operation are legal.
   */
/* @ Function: handleOperation
   @ Arguments:
   @ Description:
   */
Bool parseOperands(char *src, char *des, Operation *op, AddrMethodsOptions active[2]);

/* @ Function: handleOperation
 @ Arguments: This function gets the source and destination operands, the operation and the posible addresing method of each of the operands.
 @ Description: The function checks if the source and destination operands are valid, by checking if it has the right amount of operands, or the right addressing method.
  If the function encounter errors, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
  Returns true if the operands are valid, and false if they aren't.
 */

Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type);
/* @ Function: validateOperandMatch
   @ Arguments: This function gets AddrMethodsOptions allowedAddrs- the allowed addressing methods for the source and destination operands, AddrMethodsOptions active[2]- an array that represent types of addressing methods for the operands,
    char *operand- the operands, and type- the type of the operand.
   @ Description: The function checks if the operand has the right addressing mrhod, returns true if it does, and false if it doesn't.
   */