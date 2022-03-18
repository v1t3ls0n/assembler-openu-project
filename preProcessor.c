

#include "data.h"
extern State globalState;
extern Bool isLabelDeclaration(char *s);
extern Bool isOperation(char *s);
extern Bool isComment(char *s);
extern Bool isInstructionStrict(char *s);
extern Bool verifyLabelNaming(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern Item *getMacro(char *s);

extern void printMacroTable();
extern int printMacroItem(Item *item);
extern unsigned currentLine;

void saveMacros(FILE *target);
void popLastToken(FILE *target, int len);
void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
FILE *createExpandedSourceFile(FILE *source, char *fileName);

Bool isMacroOpening(char *s)
{

    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char *s)
{
    return !strcmp(s, "endm") ? True : False;
}
Bool isPossiblyUseOfMacro(char *s)
{
    return !isLabelDeclaration(s) && !isOperation(s) && !isComment(s) && !isInstructionStrict(s) && !isMacroClosing(s) && !isMacroOpening(s);
}

Bool isLegalMacroName(char *s)
{
    return !isInstructionStrict(s) && !isOperation(s) ? True : False;
}

void parseSourceFile(FILE *source, char *filename)
{

    FILE *target = createExpandedSourceFile(source, filename);
    rewind(target);
    fclose(target);
    printMacroTable();
}

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    ParseState state = evalToken;
    Bool isMacroCurrentlyParsed = False;
    Bool isMacroStartFoundYet = False;
    int c = 0, i = 0, j = 0, spaces = 0, current = 0;
    int start = 0, end = 0;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(source)) != EOF)
    {
        if (!isMacroCurrentlyParsed && state != parsingMacroName)
            fputc(c, target);
        if (c == '\n')
        {
            currentLine++;
            if (state == skipLine)
                state = evalToken;
        }
        spaces++;

        if (state != skipLine)
        {
            if (isMacroCurrentlyParsed && !isMacroStartFoundYet)
            {
                start = ftell(source);
                isMacroStartFoundYet = True;
            }

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

                        if (isMacroOpening(token))
                        {
                            popLastToken(target, j + i);

                            state = parsingMacroName;
                            isMacroStartFoundYet = False;
                        }
                        else
                        {
                            if (isMacroCurrentlyParsed && isMacroClosing(token))
                            {

                                popLastToken(target, i + j);
                                printf("token:%s\n", token);
                                addMacro(macroName, start, (ftell(source)));
                                memset(macroName, 0, i);
                                isMacroCurrentlyParsed = False;
                                start = end = spaces = 0;
                            }

                            else if (isPossiblyUseOfMacro(token))
                            {
                                Item *p = getMacro(token);
                                printf("token:%s\n", token);
                                if (p != NULL)
                                {
                                    /* fseek(target, -strlen(token), SEEK_CUR); */

                                    current = ftell(source);
                                    fseek(target, 0 - strlen(token), SEEK_CUR);
                                    replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
                                    fseek(source, current - 1, SEEK_SET);
                                }
                            }

                            state = skipLine;

                            if (isMacroCurrentlyParsed && isMacroStartFoundYet)
                                end++;
                        }
                    }

                    else if (state == parsingMacroName)
                    {
                        /*           popLastToken(target, i + j);
                         */
                        if (isLegalMacroName(macroName))
                        {

                            isMacroCurrentlyParsed = True;
                            state = evalToken;
                        }
                        else
                        {
                            yieldError(illegalMacroNameUseOfSavedKeywords);
                            memset(macroName, 0, i);
                            isMacroCurrentlyParsed = isMacroStartFoundYet = False;
                            i = 0;
                            state = skipLine;
                        }
                    }
                    memset(token, 0, j);
                    i = j = spaces = 0;
                }
            }
        }
    }
}

void popLastToken(FILE *target, int len)
{
    printf("inside popLastToken, len:%d\n", len);
    fseek(target, -len + 1, SEEK_CUR);
}
void replaceWithMacro(FILE *target, FILE *source, int start, int end)
{
    int c, i;
    fseek(source, start, SEEK_SET);
    fseek(target, 0, SEEK_CUR);

    for (i = end - start; i > 0 && (c = fgetc(source)) != EOF; i--)
    {
        printf("replace with macro inside loop, i:%d\n", i);
        fputc(c, target);
    }
}

/* void parseMacro(FILE *fp)
{
} */

FILE *createExpandedSourceFile(FILE *source, char *fileName)
{
    FILE *target;
    /*     int c = 0; */
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    parseAndReplaceMacros(source, target);
    printf("File copied successfully.\n");
    fclose(source);
    return target;
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
int parseAllStrict(FILE *target)
{
    ParseState state = evalToken;
    Bool isMacro = False;
    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(target)) != EOF)
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