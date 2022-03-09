/*

Symbol table is fully updated with all of the symbols with correct
values (.data/.string/.entry arguments address is updated as well to start after last
operation address)


 */
#include "data.h"
/* from variables.c (global Variables) */
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

/* from firstRun.c */
extern void parseSingleLine(char *line);
extern ParseState handleFirstToken(char *token, char *line, ParseState state);
extern Bool parseOperands(char *src, char comma, char *des, Operation *op, AddrMethodsOptions active[2]);

/* from table.c: */
extern int getSymbolBaseAddress(char *name);
extern int getSymbolOffset(char *name);
extern Bool isExternal(char *name);

/* from operation.c */
extern Operation operations[OP_SIZE];
extern Operation *getOperationByName(char *s);

/* from memory.c */
extern unsigned getDC();
extern unsigned getIC();
extern void writeIntegerIntoDataMemoryBinaryImg(int number);
extern void writeIntoDataBinaryImg(char s[BINARY_WORD_SIZE]);
extern void writeIntoCodeBinaryImg(char s[BINARY_WORD_SIZE]);

/* from encode.c */
extern char *generateFirstWordEncodedToBinary(Operation *operation);

int secondRunParseSource(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    currentLine = 1;

    printf("Second Run:\n");
    while (((c = fgetc(fp)) != EOF))
    {

        if (c == '\n')
        {
            parseSingleLineSecondRun(line);
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        else if (isspace(c))
            line[i++] = ' ';

        else
        {
            if (isprint(c))
                line[i++] = c;
        }
    }

    printf("line: %s length: %d \n", line, (int)strlen(line));
    if (i > 0)
    {
        parseSingleLine(line);
        memset(line, 0, i);
    }

    return True;
}

Bool writeOperationBinary(char *operationName, char *line)
{
    Operation *p = getOperationByName(operationName);
    char firstOperand[MAX_LABEL_LEN] = {0}, secondOperand[MAX_LABEL_LEN] = {0};
    char comma = 0;
    int nTotal = 0, nFirst = 0;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
    printf("writeOperationBinary in second run line 84\n");
    line = operationName + strlen(operationName) + 1;
    sscanf(line, "%s%n%c%s%n", firstOperand, &nFirst, &comma, secondOperand, &nTotal);
    if (secondOperand[0] == 0 && firstOperand[0] != 0)
    {
        if (!strchr(firstOperand, ','))
        {
            memcpy(secondOperand, firstOperand, nFirst);
            firstOperand[0] = '\0';
        }
        else
        {
            memcpy(secondOperand, strrchr(firstOperand, ','), strlen(firstOperand));
            firstOperand[strlen(firstOperand) - strlen(secondOperand)] = 0;
        }
    }

    if (parseOperands(firstOperand, comma, secondOperand, p, active))
    {

        unsigned srcBase = 0, srcOffset = 0, desBase = 0, desOffset = 0;
        printf("line 183, handle Operation\n");
        printf("active:\nSRC: direct:%u index:%u immediate:%u reg:%u\n", active[0].direct, active[0].index, active[0].immediate, active[0].reg);
        printf("DES: direct:%u index:%u immediate:%u reg:%u\n", active[1].direct, active[1].index, active[1].immediate, active[1].reg);
        /*
                writeFirstWord(p); */

        /*         if (active[0].direct || active[0].index)
                {

                    srcBase = getSymbolBaseAddress(firstOperand);
                    srcOffset = getSymbolOffset(firstOperand);

                    if (!srcBase || !srcOffset)
                    {
                        globalState = secondRunFailed;
                        return yieldError(labelNotExist);
                    }
                    else
                        writeDirectOperand(srcBase, srcOffset, isExternal(firstOperand) ? E : R);
                }
                if (active[1].direct || active[1].index)
                {
                    desBase = getSymbolBaseAddress(secondOperand);
                    desOffset = getSymbolOffset(secondOperand);
                    if (!desBase || !desOffset)
                    {
                        globalState = secondRunFailed;
                        return yieldError(labelNotExist);
                    }

                    else
                        writeDirectOperand(desBase, desOffset, isExternal(firstOperand) ? E : R);
                }
             */
    }

    return True;
}

void writeDirectOperand(unsigned base, unsigned offset, int _ARE)
{

    writeIntoCodeBinaryImg(strcat(hexToBin(decToHex(_ARE)), hexToBin(decToHex(base))));
    writeIntoCodeBinaryImg(strcat(hexToBin(decToHex(_ARE)), hexToBin(decToHex(offset))));
}

void writeFirstWord(Operation *operation)
{

    /* writeIntoCodeBinaryImg(generateFirstWordEncodedToBinary(operation));
     */
}

void writeSecondWord()
{

    char binaryString[BINARY_WORD_SIZE] = {"00000000000000000000"};

    writeIntoCodeBinaryImg(binaryString);
}

Bool writeInstructionBinary(char *instructionName, char *line)
{
    printf("writeInstructionBinary in second run line 170\n");

    return True;
}

void parseSingleLineSecondRun(char *line)
{
    ParseState state = newLine;
    char *p = calloc(strlen(line + 1), sizeof(char *));
    char *token = calloc(MAX_LABEL_LEN, sizeof(char *));

    printf("\ninside parseSingleLine, Line Number (%d):\n%s\n", currentLine, line);

    memcpy(p, line, strlen(line));
    token = strtok(p, " \t \n");
    state = handleFirstToken(token, p, state);

    while (token != NULL)
    {

        switch (state)
        {

        case writingOperationIntoMemoryImg:
        {
            state = writeOperationBinary(token, line);
            break;
        }

        case writingDataIntoMemoryImg:
        {
            state = writeInstructionBinary(token, line);

            break;
        }

        case Err:
        {
            if (globalState == firstRun)
                globalState = collectErrors;

            state = Err;
            break;
        }

        case skipLine:
            state = lineParsedSuccessfully;

        default:
            break;
        }
        token = strtok(NULL, " \t \n");
    }

    currentLine++;
    free(p);
    free(token);
}

ParseState handleSecondRunFirstToken(char *token, char *line, ParseState state)
{
    /*   printf("inside handle State, token:%s\n", token); */

    printf("firstRun.c, line 138\n");

    switch (state)
    {
    case skipLine:
        return lineParsedSuccessfully;

    case newLine:
    {
        if (token[0] == ';')
            return skipLine;

        if (isLabel(token))
        {
            token = strtok(NULL, " \t \n");
            if (isInstruction(token))
                return writingDataIntoMemoryImg;

            else if (isOperation(token))
                return writingOperationIntoMemoryImg;
        }
        else if (isInstruction(token))
            return writingDataIntoMemoryImg;

        else if (isOperation(token))
            return writingOperationIntoMemoryImg;
    }

    default:
        break;
    }
    return True;
}
