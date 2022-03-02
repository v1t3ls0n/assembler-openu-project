

#include "data.h"
extern State globalState;

void parseSourceFile(FILE *fp, char *filename)
{
    int c = 0;
    int start = 0, end = 1;
    char line[MAX_LINE_LEN + 1] = {0};
    while ((c = fgetc(fp)) != EOF)
    {

        if (c == '\n')
        {
            fgets(line, end - start, fp);
        }
        end++;
    }

    /*
    1 - we create new file for the extended source file without the macros,
    we will write the output in the new file.
we will read line by line from source file,

     */
}

int readFromFileByIndexes(FILE *fptr, char *filename, int start, int end)
{
    int c;
    if (fseek(fptr, end - start, start) != 0 || (c = fgetc(fptr)) == EOF)
    {
        /* yield some error abou


         */
    }

    return 1;
}
void replaceWithMacro(FILE *p, int startIndex, int endIndex)
{
}

void parseMacro(FILE *fp)
{
}
