#include "data.h"

/* @ Function: void parseSourceFile(FILE *src, FILE *target)
   @ Arguments: FILE *src, FILE *target. the *src FILE pointer
   is a file pointer to the original source code file (".as" file) and the *target FILE
   pointer argument is for an empty file that will be the expanded source file version of the src
   file without the macros (target is the ".am" file we will generate from source in this function);
   @ Description: This function splits the source file content to different lines, and it pass each
   seperated line to the parseMacros(char *line, char *token, FILE *src, FILE *target)
*/
void parseSourceFile(FILE *src, FILE *target);

/* @ Function: void parseMacros(char *line, char *token, FILE *src, FILE *target);
   @ Arguments: char *line, char *token, FILE *src, FILE *target
   *src and *target FILE pointers are the same as in the function we mentioned above.
   line is the copy of the current line we parsed from the source file and token is the
   first non-empty space token found in that line.
   @ Description: this function is invoked by parseSourceFile function and is in take write the
   line into the new ".am" expanded file after checking if there are macros on line (macro opening/closing/mentioning)
   and ignoring/replacing them correspondingly.
*/
void parseMacros(char *line, char *token, FILE *src, FILE *target);
