/*
int num = 0;
char *p = s;
p++;
num = atoi(p);

printf("inside getRegisteryNumber, number:%d\n", num);
if (operand[0] == 'r' && isdigit(operand[strlen(operand) - 1]))
    yieldError(wrongRegisteryReferenceUndefinedReg);
 */

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
            currentLineNumber++;
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
                  if ((currentLineNumber - start) < 2)
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


return target;
}
* /