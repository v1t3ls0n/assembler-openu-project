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
int main()
{
    char str[50] = "        XYZ:  .data 7,-57,+17";
    int c = NULL;
    int i = 0, j = 0;
    char line[81];
    char token[81];

    State state = ignore;
    Parse parse = any;
    Expect expect = any;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            if (state == ignore)
            {
                /*
                build Words from line
                */
            }

            switch (state == parse)
            {
            }
        }

        if (!isspace(c))
        {

            if (state == ignore)
            {

                switch (c)
                {

                case '.':
                {
                    parse = instruction;
                    expect = character;
                }

                case '+':
                {
                    parse = number;
                    expect = number;
                }
                case '-':
                {
                    parse = number;
                    expect = number;
                }

                default:
                    break;
                }
            }

            else if (state == parse)
            {

                switch (expect)
                {

                    /*
                         label,
                        number,
                        macro,
                        array,
                        instruction,
                        dataInstuction,
                        command,
                        comma,
                        any
                    */
                case comma:
                {
                }

                case number:
                {
                }

                case character:
                {
                }

                case instruction:
                {
                }

                default:
                    break;
                }
            }
            state = parse;
            token[j++] = (char)c;
        }
        else if (isspace(c) && state == parse)
        {

            j = 0;
            expect = any;
            memset(token, 0, j);
            state = ignore;
        }
        else if (isspace(c) && state == ignore)
        {
            /*
            do nothing
            */
        }
    }
}