#include "parse.c"

/* @ Function: verifyCommaSyntax
   @ Arguments:
   @ Description:
*/
Bool verifyCommaSyntax(char *line);

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the number of .data elements that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .data instruction will take.
*/
Bool countAndVerifyDataArguments(char *line);

/* @ Function: countAndVerifyStringArguments
   @ Arguments: the function gets char * token which is the current token that we are about to parse the string argument from.
   @ Description: The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors no opening or closing quotes, it yields (prints) the relevant error message.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/
Bool countAndVerifyStringArguments(char *line);

/* @ Function: parseLine
   @ Arguments: The function gets char * token which is the current token that we are about to parse and char *line which is the current line being parsed
   @ Description: The function checks what is the current globalState, than checks what is the current token (an instruction, an operation, a label declaration...)
   The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors no opening or closing quotes, it yields (prints) the relevant error message.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/

/* @ Function: parseLine
   @ Arguments:
   @ Description:
*/
Bool parseLine(char *token, char *line);

/* @ Function: handleSingleLine
   @ Arguments:
   @ Description:
*/
Bool handleSingleLine(char *line);

/* @ Function:parseAssemblyCode
   @ Arguments:
   @ Description:
*/
void parseAssemblyCode(FILE *src);
