
/*
##########################################################################################################
##########################################################################################################
#################################### secondRun.c General Overview: #######################################
##########################################################################################################
##########################################################################################################

In the second run our assembler writes the words of each line of code in binary format, we inserting the words to
the memory image in the correct position depending on the type of word (data/instruction) and we add each external
operand appearence to the linked list of all the positions that external operands appeared. if we encouter label
operand that is not within the symbol table and is not external we yield error message and continue checking the
rest of the code in order to discover all errors of this type if this case happens we will finish second run but
will not export any files.

##########################################################################################################
##########################################################################################################
##########################################################################################################
*/

Bool writeOperationBinary(char *operationName, char *args);
/* @ Function: writeOperationBinary
   @ Arguments: The function gets char *operationName- the name of the operation and char *args - the arguments of the operation
   @ Description: The function splits char * args to tooken- each token is a different operand.
   Then the function checks what is the kind of the operands and writes them in words at the memory.
   If there is an operand which is an undifined labl at the table of symbols, it returns false. Else it returns true.
*/

void writeAdditionalOperandsWords(const Operation *op, AddrMethodsOptions active, char *value);
/* @ Function: writeAdditionalOperandsWords
   @ Arguments: The function gets const Operation *op- the operation, AddrMethodsOptions active- the allowed addresing methods for the operands,
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

void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], const Operation *op);
/* @ Function: writeSecondWord
   @ Arguments: The function gets char *first and char *second - the first and second operands of the operation- const Operation *op.
   It also gets AddrMethodsOptions active[2]- an array with the allowed addresing methods for the operands.
   @ Description: The function builds and writes in the memory the second word of each operation and its parameters, it checks what is the addressing method of first and second, what is the funct of the operation.
    The function doesn't return value.
*/

void writeFirstWord(const Operation *op);
/* @ Function: writeFirstWord
   @ Arguments: The function gets const Operation *op- the operation it writes uts first word.
   @ Description: The function writes in the memory the first word of the operation and its parameter by writing the opcode of the operation into the memory.
    The function doesn't return value.
*/

void writeDirectOperandWord(char *labelName);
/* @ Function: writeDirectOperandWord
   @ Arguments: The function gets char *labelName which is the label it will update the the external position table.
   @ Description: The function updates the external operands position table that we will gnerate from the .ext files.
    The function doesn't return value.
*/

void writeImmediateOperandWord(char *n);
/* @ Function: writeImmediateOperandWord
   @ Arguments: The function gets char *n- the immmediate operand it will add to the memory.
   @ Description: The function adds the immediate operand's numeric value by using the function addWord which will write it in the memory.
   The function doesn't return value.
*/

Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type);
/* @ Function: detectOperandType
   @ Arguments: The function gets char *operand- the operand it checks it's type, AddrMethodsOptions active[2]- an array with the allowed addresing methods for the operand, and int type- the type of the operand (source or destination).
   @ Description: The function checks what is the addressing method for the operand.
    If it is a label (direct parameter), it checks if it is exist in the table symbol (if not it yields error) and if the operand is both entry and externl which is illegal, and then it yeilds (prints) error too.
    Returns true if the operation is valid, and false if it isn't.
*/

char *parseLabelNameFromIndexAddrOperand(char *s);
/* @ Function: parseLabelNameFromIndexAddrOperand
   @ Arguments: The function gets char *s, which is the operand from index addressing method it parses.
   @ Description: The function parses out the label name from the char *s it gets.
   Returns the label name from the token it got.
*/

int parseRegNumberFromIndexAddrOperand(char *s);
/* @ Function: parseRegNumberFromIndexAddrOperand
   @ Arguments: The function gets char *s, which is the operand from index addressing method it parses.
   @ Description: The function parses out the register's number from the char *s it gets.
   Returns the register's number from the token it got.
*/