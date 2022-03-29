
/*
   ##########################################################################################################
   ###########################################  General Overview: ###########################################
   ##########################################################################################################

   In FirstRun.c we are parsing and analysizing the assembly code we got after we extracted the macros from it
   we verify that the syntax is legal and that the use of the assembly language follows the rules of our
   operations and instructions (operands type match, instruction type match, comment type match, legal declaretion
   of symbols, etc). If we encounters error we yields (print to error.log file and to stderr) the specific error and
   we continue checking next characters in line. For each line of code.

   If no errors encountered then we will only INCREASE the DATA COUNTER or the INSTRUCTION COUNTER
   correspondingly by the size that the line of code should take in memory (we calculate the size
   of each instruction or operation in this proccess as well).

   By doing so, at the end of the first run we will know exacly how much space we will need for the data image and how
   much space we need for the instruction image. Doing so allows us to allocate just the required amount of memory
   needed without wasting big random chuncks of memory upfront. If everything goes well, means that the  first Run end
   without errors, then we set the sharedState ((*globalState)()) to secondRun.

   ##########################################################################################################
   ##########################################################################################################
   ##########################################################################################################
   ##########################################################################################################

 */

/* @ Function: handleOperation
   @ Arguments: The function gets char *operationName which the name of the current operation and char * args which are the arguments of the operation.
   @ Description:This function is the main function of parsing operations. It splits the operands from the line, it calls to the functions to validate them.
   For instance, it calls to parseOperand to validate that all of the operation are legal.
   */
/* @ Function: handleOperation
   @ Arguments:
   @ Description:
   */
Bool handleOperation(char *operationName, char *args);
/* @ Function: handleOperation
 @ Arguments: This function gets the source and destination operands, the operation and the posible addresing method of each of the operands.
 @ Description: The function checks if the source and destination operands are valid, by checking if it has the right amount of operands, or the right addressing method.
  If the function encounter errors, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
  Returns true if the operands are valid, and false if they aren't.
 */

Bool parseOperands(char *src, char *des, const Operation *op, AddrMethodsOptions active[2]);
/* @ Function: validateOperandMatch
   @ Arguments: This function gets AddrMethodsOptions allowedAddrs- the allowed addressing methods for the source and destination operands, AddrMethodsOptions active[2]- an array that represent types of addressing methods for the operands,
    char *operand- the operands, and type- the type of the operand.
   @ Description: The function checks if the operand has the right addressing mrhod, returns true if it does, and false if it doesn't.
   */
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type);

Bool handleInstruction(int type, char *firstToken, char *nextTokens, char *line);
/* @ Function: Bool handleInstruction(int type, char *firstToken, char *nextTokens, char *line)
   @ Arguments: the function gets int type- the type of the instruction,
   char *firstToken and char *nextToken- the first and second tokens in the instruction line (char * line).
   @ Description: The function checks the first token. If it is an instruction,
   it checks what is the type of the instruction, and yeilds error if there are illegal appearence of characters in the declaration.
   If it is a label declaration, it makes sure there is a valid instruction
   and its arguments afterwards, if the declaration is not valid it yeilds error.
   If the firstToken is not a label declaration or any type of legal instruction, it yeilds error.
   The function keeps on validating the whole line and reports all of the errors.
   If there aren't any mistake, the function returns true and if there is a vald label it adds it to the symbols table.
   Else, it returns false.

*/

Bool handleLabel(char *labelName, char *nextToken, char *line);
/*  @ Function: Bool handleLabel(char *labelName, char *nextToken, char *line)
    @ Arguments: The function gets char *labelName- the name of the label it handles,
    char *nextToken- one of the tokens in the line (char * line).
    @ Description: The function gets a line in the code that starts with a label declaration.
    The function checks if this is an instruction line or an operation line.
    If it encountered an error it yields (prints) the error.
     The function keeps on validating the whole line and reports all of the errors.
    The function returns true if the line is valid, andfalse if it ian't.
*/