#include "table.h"

int main()
{
    return 0;
}

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;
}

Item *lookup(char *s, ItemType type)
{

    Item *np;
    for (np = (type == Symbol ? symbols[hash(s)] : macros[hash(s)]); np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

Item *install(char *name, ItemType type)
{
    unsigned hashval;
    Item *np;
    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }
    /* Key name already exist inside table, yield Error of duplicate values in symbol/macro table*/
    else
        return NULL;

    return np;
}

void setSymbolData(Item *symbol, unsigned value, Attribute *attrs)
{
    unsigned base;
    unsigned offset;
    int attrsCount, i = 0;
    /* calculate base and offset from value*/
    /* calculate attrs array size save to attrsCount*/
    /*meanwhile I will put some generic values in those variables*/
    base = 96;
    offset = 4;
    attrsCount = 5;

    symbol->val.s.base = base;
    symbol->val.s.offset = offset;
    for (i = 0; i < attrsCount; i++)
    {
        /* malloc size for symbol->attrs array*/
        symbol->val.s.attrs[i] = attrs[i];
    }
}

void setMacroData(Item *macro, char *code)
{
    macro->val.m.code = code;
}
