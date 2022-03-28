#include "data.h"

char *cloneString(char *s)
{
    char *copy;
    if (!(*s))
        return "";
    copy = (char *)calloc(strlen(s), sizeof(char *));
    strcpy(copy, s);
    return copy;
}

char *trimFromLeft(char *s)
{
    while (isspace(*s) && *s != '\0')
        s++;
    return s;
}

char *decToHex(int num)
{
    int i = num, size = 0;
    char *hex;
    for (size = 0; i > 0; i = i / 16)
        size++;
    hex = (char *)calloc(size, sizeof(char *));
    sprintf(hex, "%05x", num);
    return hex;
}
