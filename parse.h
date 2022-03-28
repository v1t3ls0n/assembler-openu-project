#include "parse.c"

/* @ Function: verifyCommaSyntax
   @ Arguments:
   @ Description:
*/
Bool verifyCommaSyntax(char *line);
/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to verify if the comma syntax is legal or not.
   @ Description: The function splits the line into tokens (operands and commas), it checks if between every two arguments there is exactly one comma.
   If there is an illegal comma, a missing comma or an extra comma, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   Returns true if the line is valid and false if it isn't.
*/

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the number of .data elements that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .data instruction will take.
*/
Bool countAndVerifyDataArguments(char *line);

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors, an illegal comma or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the number of .data elements that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .data instruction will take.
*/

/* @ Function: countAndVerifyStringArguments
   @ Arguments: the function gets char * token which is the current token that we are about to parse the string argument from.
   @ Description: The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors no opening or closing quotes, it yields (prints) the relevant error message.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/
Bool countAndVerifyStringArguments(char *line);
/* @ Function: countAndVerifyStringArguments
   @ Arguments: the function gets char * token which is the current token that we are about to parse the string argument from.
   @ Description: The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors- no opening or closing  (or no opening and closing) quotes, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/

/* @ Function: parseLine
   @ Arguments:
   @ Description:
*/
Bool parseLine(char *token, char *line);
/* @ Function: parseLine
   @ Arguments: The function gets char * token which is the first token of the line that we are about to parse and char *line which is the current line being parsed
   @ Description: The function checks what is the current globalState, than checks what is the first token (an instruction, an operation, a label declaration...)
    If the globalState is equal to firstRun, according to the first token, it finds out what kind of line is the current line and calls other function so they can handle the line.
    If the globalState is equal to secondRun, according to the first token, it calls function to write the compiled memory.
    If it finds some errors, it yields (prints) the relevant error message.
    If the line turns out to be valid, it returns true, else it returns false.
*/
/* @ Function: handleSingleLine
   @ Arguments:
   @ Description:
*/
Bool handleSingleLine(char *line);
/* @ Function:handleSingleLine
   @ Arguments: This function gets char *line which is the current line that is being parsed.
   @ Description: The function checks what is the current globalState, and by that decides how to split the line into tokens (what is the delimeter).
   Than the function calls the function parseLine in order to check if there are errors in the line.
   Than its increases by 1 the number of line it parses (for the next line to come).
   Returns true if the line is valid and false if it isn't.
*/

void parseAssemblyCode(FILE *src);
/* @ Function:parseAssemblyCode
   @ Arguments: The function gets FILE *src which is a file after the pre proccesoring.
   @ Description: The function parses the file, splits it to tokens and calls the right functions to complete the first and second run.
*/