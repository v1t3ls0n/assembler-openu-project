#include "sharedStates.c"

void setGlobalState(State newState);
/* @ Function: setGlobalState
   @ Aruments: The function gets State new- the state the function will update
   @ Description: The function updates the globalState to be the state it gets. This is the one of the only times we have an access to the global state (only through using the functions setGlobalState and getGlobalState)
    The function doesn't return any value.
*/

State getGlobalState();
/* @ Function: getGlobalState
   @ Aruments: The function doesn't get any arguments.
   @ Description: The function gets the global state. This is the one of the only times we have an access to the global state (only through using the functions setGlobalState and getGlobalState)
    The function returns the current global state.
*/

void setFileNamePath(char *s);
/* @ Function: setFileNamePath
   @ Aruments: The function gets char *s which is a current file name that is handled by our assembler at the moment.
   @ Description: This is a setter function that updates te state of the current file name path, this state is used
   in other files of the program that needs to know what is the base file name and exact path of the current file that is being handled.
    The function doesn't return any value.
*/

char *getFileNamePath();
/* @ Function: getFileNamePath
   @ Aruments: The function doesn't get any arguments.
   @ Description: The function is a getter function for the state of the current file name path.
   The function returns separated copy of the string (clone), in order for other parts of the program
   that are also dealing with creating an outputing different file outputs (.ob, .ext, .ent, yieldError and more)
    for each file input the program gets fromthe user.
*/

void resetCurrentLineNumber();
/* @ Function: resetCurrentLineNumber
   @ Aruments: The function doesn't get any arguments.
   @ Description: The function resets currentLineNumber to be 1.
    The function doesn't return any value.
*/

void increaseCurrentLineNumber();
/* @ Function: increaseCurrentLineNumber
   @ Aruments: The function doesn't get any arguments.
   @ Description: The function increases currentLineNumber by 1.
    The function doesn't return any value.
*/

int getCurrentLineNumber();
/* @ Function: getCurrentLineNumber
   @ Aruments: The function doesn't get any arguments.
   @ Description: The function increases currentLineNumber by 1.
    The function doesn't return any value.
*/