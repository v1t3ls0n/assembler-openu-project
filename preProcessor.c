

#include "data.h"
extern State globalState;
extern Bool isLabelDeclaration(char *s);
extern Bool isOperation(char *s);
extern Bool isComment(char *s);
extern Bool isInstruction(char *s);
extern Bool verifyLabelNaming(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern void printMacroTable();
extern int printMacroItem(Item *item);
extern unsigned currentLine;

void saveMacros(FILE *newFile);

Bool isMacroOpening(char *s)
{
    /*     printf("in is macro opening s:%s\n", s);
     */
    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char *s)
{
    /*     printf("in is macro closing, s:%s\n", s); */
    return !strcmp(s, "endm") ? True : False;
}

Bool isLegalMacroName(char *s)
{
    return (strcmp(s, DATA) && strcmp(s, STRING) && strcmp(s, EXTERNAL) && strcmp(s, ENTRY)) && !isOperation(s) ? True : False;
}

void parseSourceFile(FILE *source, char *filename)
{

    FILE *newFile = createCopyFromSourceFile(source, filename);
    rewind(newFile);
    saveMacros(newFile);
    fclose(newFile);
    printMacroTable();
}

void saveMacros(FILE *newFile)
{
    ParseState state = evalToken;
    Bool isMacroCurrentlyParsed = False;
    int c = 0, i = 0, j = 0;
    int start = 0, end = 0, current = 0;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(newFile)) != EOF)
    {
        current++;
        if (c == '\n')
        {
            currentLine++;
            if (state == skipLine)
                state = evalToken;
        }
        if (state != skipLine)
        {

            if (!isspace(c))
            {
                if (j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
                {
                    if (state == parsingMacroName)
                        macroName[i++] = c;

                    else
                        token[j++] = c;
                }
            }
            else
            {
                if (j > 0 || i > 0)
                {
                    if (state == evalToken)
                    {
                        if (!isMacroCurrentlyParsed && isMacroOpening(token))
                        {
                            state = parsingMacroName;
                            start = current;
                            isMacroCurrentlyParsed = True;
                        }

                        else if (isMacroCurrentlyParsed && isMacroClosing(token))
                        {
                            end = current;
                            addMacro(macroName, start, end);
                            start = end = current;
                            memset(macroName, 0, i);
                            isMacroCurrentlyParsed = False;
                            state = skipLine;
                        }
                        else
                            state = skipLine;
                    }

                    else if (state == parsingMacroName)
                    {

                        if (isLegalMacroName(macroName))
                            state = evalToken;
                        else
                        {
                            yieldError(illegalMacroNameUseOfSavedKeywords);
                            memset(macroName, 0, i);
                            state = skipLine;
                            isMacroCurrentlyParsed = False;
                        }
                    }
                    memset(token, 0, j);
                    i = j = 0;
                }
            }
        }
    }
}

int parseNextLine(int start, int end)
{

    /*     printf("start:%d\nend:%d\n", start, end); */
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
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    while ((c = fgetc(source)) != EOF)
    {

        fputc(c, target);
    }

    printf("File copied successfully.\n");
    fclose(source);
    /*     fclose(target); */

    return target;
}
/*
int parseAllStrict(FILE *newFile)
{
    ParseState state = evalToken;
    Bool isMacro = False;
    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(newFile)) != EOF)
    {
        current++;
        if (c == '\n' && state == skipLine)
        {
            if (state == skipLine)
                state = evalToken;

            current++;
            currentLine++;
        }

        if (state != skipLine)
        {

            if (!isspace(c) && j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
            {
                if (state == parsingMacroName)
                    macroName[i++] = c;

                else
                    token[j++] = c;
            }
            else if (isspace(c) && (j > 0 || i > 0))
            {

                if (i > 0)
                    macroName[i] = '\0';
                if (j > 0)
                    token[j] = '\0';


                if (state == evalToken)
                {
                    if (!isMacro && isMacroOpening(token))
                    {
                        state = parsingMacroName;
                        isMacro = True;
                    }
                    if (!isMacro && isMacroClosing(token))
                    {
                        yieldError(macroClosingWithoutAnyOpenedMacro);
             state = skipLine;
          }

          else if (isMacro)
          {

              if (isMacroClosing(token))
              {
                  end = current;

                  updateMacro(macroName, start, end);
                  memset(macroName, 0, i);
                  i = 0;
                  isMacro = False;
                  state = skipLine;
              }

              else if (isMacroOpening(token))
              {
                  if ((currentLine - start) < 2)
                  {
                      yieldError(useOfNestedMacrosIsIllegal);
                  }
                  updateMacro(macroName, -1, -1);
                  memset(macroName, 0, i);
                  i = 0;
                  isMacro = False;
                  state = skipLine;
              }
          }

          memset(token, 0, j);
          j = 0;
      }

      else if (state == parsingMacroName)
      {

          if (isLegalMacroName(macroName))
          {
              start = current;
              addMacro(macroName, start, -1);
              i = 0;
              state = evalToken;
          }
          else
          {
              yieldError(illegalMacroNameUseOfSavedKeywords);
              memset(macroName, 0, i);
              i = 0;
              state = skipLine;
              isMacro = False;
          }
      }
  }
}
}

return 1;
}

*/