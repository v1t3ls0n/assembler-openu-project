#include "data.h"

/* @ Function: void parseSourceFile(FILE *src, FILE *target)
   @ Arguments: FILE *src, FILE *target. the *src FILE pointer
   is a file pointer to the original source code file (".as" file) and the *target FILE
   pointer argument is for an empty file that will be the expanded source file version of the src
   file without the macros (target is the ".am" file we will generate from source in this function);
   @ Description: This function splits the source file content to different lines, and it pass each
   seperated line to the parseMacros(char *line, char *token, FILE *src, FILE *target) function which
   is in take
*/
void parseSourceFile(FILE *src, FILE *target);

void parseMacros(char *line, char *token, FILE *src, FILE *target);
