

#include "data.h"
extern State globalState;

void parseSourceFile(FILE *source, char *filename)
{
    /*
1 - we create new file for the extended source file without the macros,
we will write the output in the new file.
we will read line by line from source file,

*/
    int c = 0;
    int i = 0;
    int start = 0, end = 1;
    char line[MAX_LINE_LEN + 1] = {0};
    /*     ParseState state = newLine; */
    FILE *newFile = createCopyFromSourceFile(source, filename);

    while ((c = fgetc(newFile)) != EOF)
    {
        if (c == '\n' || c == ';' || i == MAX_LINE_LEN)
        {
            parseNextLine(line, start, end);
            memset(line, '\0', i);
            i = 0;
            start = end;
            end++;
        }
        if (!isspace(c))
            line[i++] = c;
        end++;
    }
}

int parseNextLine(char *line, int start, int end)
{
    return 1;
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

FILE *createCopyFromSourceFile(FILE *source, char *fileName)
{
    FILE *target;
    int c = 0;
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    while ((c = fgetc(source)) != EOF)
        fputc((char)c, target);

    printf("File copied successfully.\n");
    fclose(source);
    fclose(target);

    return target;
}