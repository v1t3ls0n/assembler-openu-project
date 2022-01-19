#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    ignore,
    parse,
    error
} State;

typedef enum
{
    comma,
    space,
    newLine,
    macro,
    array,
    label,
    character,
    number,
} Expect;
typedef enum
{
    label,
    number,
    macro,
    array,
    instruction,
    dataInstuction,
    command,
    comma,
    any
} Parse;

int getFunctCode();
int getOpcode();
void parseLine();
void updateDataTable();
