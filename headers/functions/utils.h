
/*
##########################################################################################################
##########################################################################################################
#################################### utils.c/utils.h General Overview: ###################################
##########################################################################################################
##########################################################################################################

Utils.c files contains most of all helpers functions that are used for verifying and checking and
validating tokens identity, funtion here are mainly used out side of this file by other functions that deals
with parsing the assembly code.

##########################################################################################################
##########################################################################################################
##########################################################################################################
*/

Bool isMacroOpening(char *s);
/* @ Function: isMacroOpening
   @ Arguments: The function gets a char *s, the string it checks whether it is the opening of a macro or not.
   @ Description: The function checks if char *s is equal to "macro", returns true if it is, and false if it isn't
*/

Bool isMacroClosing(char *s);
/* @ Function: isMacroClosing
   @ Arguments: The function gets a char *s, the string it checks whether it is the closing of a macro or not.
   @ Description: The function checks if char *s is equal to "endm", returns true if it is, and false if it isn't
*/

Bool isPossiblyUseOfMacro(char *s);
/* @ Function: isPossiblyUseOfMacro
   @ Arguments: The function gets a char *s, the string it checks whether it can be a macro or not.
   @ Description: The function checks if char *s can be a macro, by illuminating the other things it can be (an instruction, a label...).
   Returns true if it could be a macro and false if it couldn't.
*/

Bool isLegalMacroName(char *s);
/* @ Function: isLegalMacroName
   @ Arguments: The function gets a char *s, the string it checks whether it is a legal macro name or not.
   @ Description: The function checks if char *s is a legal macro name by verifying that it's name is not a name of an instruction or an operation.
    Returns true if it is a legal macro name and false if it is not.
*/

Bool isInstruction(char *s);
/* @ Function: isInstruction
   @ Arguments: The function gets a char *s, the string it checks whether it or a part of it is an instruction or not.
   @ Description: The function checks if char *s or a part of it is an instruction by comparing it to the name of the instructions or checking if one of the instructions are a substring of it.
   Returns true if it or a part of it is an instruction, and false if it isn't.
*/

Bool isInstructionStrict(char *s);
/* @ Function: isInstructionStrict
   @ Arguments: The function gets a char *s, the string it checks whether it is an instruction or not.
   @ Description: The function checks if char *s is an instruction by comparing it to the name of the instructions.
   Returns true if it is an instruction, and false if it isn't.
*/

Bool isRegistery(char *s);
/* @ Function: isRegistery
   @ Arguments: The function gets a char *s, the string it checks whether it is a register or not.
   @ Description: The function checks if char *s is a legal register by checking if the first char is 'r' and theothers are int numbers between 0-15.
   Returns true if it is a register, and false if it isn't.
*/

Bool isValidImmediateParamter(char *s);
/* @ Function: isValidImmediateParamter
   @ Arguments: The function gets a char *s, the string it checks whether it is a valid immediate parameter or not.
   @ Description: The function checks if char *s is a valid immediate parameter by verifying the first token us a -, a + or a digit and the other vhars are digits.
   Returns true if it is a valid immediate parameter, and false if it isn't.

*/

Bool isValidIndexParameter(char *s);
/* @ Function: isValidIndexParamter
   @ Arguments: The function gets a char *s, the string it checks whether it is a valid index parameter or not.
   @ Description:The function checks if char *s is a valid index parameter by checkin if it is a label and a register (between 10-15) in parenthesis afterwards
   Returns true if it is a valid index parameter, and false if it isn't.
*/

Bool isIndexParameter(char *s);
/* @ Function: isIndexParameter
   @ Arguments: The function gets a char *s, the string it checks whether it is a index parameter or not.
   @ Description:The function checks if char *s is a valid index parameter by checkin if it is a label and a register (any register, not strict to 10-15) in parenthesis afterwards
   Returns true if it is a valid index parameter, and false if it isn't.
*/

Bool isComment(char *s);
/* @ Function: isComment
   @ Arguments: The function gets a char *s, the string it checks whether it is a comment or not.
   @ Description: The function checks if char *s is a comment by checking if it's first char is ;.
   Returns true if it is a comment, and false if it isn't.
*/
Bool isOperation(char *s);
/* @ Function: isOperation
   @ Arguments: The function gets a char *s, the string it checks whether it is an operation or not.
   @ Description: The function checks if char *s is an operation, by checking if it is equal to one of the names of the operations, using getOperationByName.
   Returns true if it is an operation, and false if it isn't.
*/

Bool isLabelDeclarationStrict(char *s);
/* @ Function: isLabelDeclarationStrict
   @ Arguments: The function gets a char *s, the string it checks whether it is a label declaration or not.
   @ Description: The function checks if char *s is a label declaration by checking if it's last char is :.
     Returns true if it is a label declaration, and false if it isn't.
*/

Bool isLabelDeclaration(char *s);
/* @ Function: isLabelDeclaration
   @ Arguments: The function gets a char *s, the string it checks whether it or a part of it is a label declaration or not.
   @ Description: The function checks if char *s or a part of it is a label declaration by checking if it consists a :.
   Returns true if it or a part of it is a label declaration, and false if it isn't.
*/

int getInstructionType(char *s);
/* @ Function: getInstructionType
    @ Arguments: The function gets a char *s, the string it checks what instruction it is.
    @ Description: The function checks if char *s or a part of it is an instruction  by  checking if one of the instructions are a substring of it.
    Returns the right instruction if it is an instruction, and false(0) if it isn't
*/

int getRegisteryNumber(char *s);
/* @ Function: getRegisteryNumber
    @ Arguments: The function gets a char *s, the string it checks from what is the number of the register.
    @ Description: The function checks what is the number of the register by checking the number at the end of the string
    Returns the number of the register.
*/

char *getInstructionName(char *s);

char *getInstructionNameByType(int type);
/* @ Function: getInstructionNameByType
   @ Arguments: The function gets int type- the type of instruction.
   @ Description: The function returns the type of the istruction and returns it as a string with \0 at the end.
   If type is not an instruction it returns NULL.
*/

Bool verifyLabelNaming(char *s);
/* @ Function: verifyLabelNaming
   @ Arguments: The function gets char *s- the name of the label it about to verify.
   @ Description: The function makes sure the name of the label is valid, by checking things like its length, if it is equal to an operation...
   Returns true if it is a valid name of label, and false if it isn't.
*/

Bool verifyLabelNamingAndPrintErrors(char *s);
/* @ Function: verifyLabelNamingAndPrintErrors
   @ Arguments: The function gets char *s- the name of the label it about to verify and prints it's errors.
   @ Description: The function makes sure the name of the label is valid, by checking things like its length, if it is equal to an operation...
   If it finds an error it yields (prints) it and returns false, else returns true.
*/
