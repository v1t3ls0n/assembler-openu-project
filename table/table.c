#include "table.h"

int main()
{
    Attributes att;
    Item *current;
    current = install("LIST", Symbol);
    att.code = 1;
    att.entry = 1;
    setSymbolData(current, 104, att);
    printSymbolTable();

    /*
    Item *p;
    p = (Item *)malloc(sizeof(Item *));
    Attribute att[] = {code, entry};
    p = install("LIST", Symbol);

    p = install("LIST", Symbol);
    if (p == NULL)
        printf("duplicate symbol!\n");
    else
    {
        att[0] = data;
        att[1] = entry;
        setSymbolData(p, 100, att);
    }

    p = install("MAIN", Symbol);
    if (p == NULL)
        printf("duplicate symbol!\n");
    else
    {
        att[0] = code;
        att[1] = entry;
        setSymbolData(p, 104, att);
    }

    printSymbolTable();
    */

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
    printf("inside lookup, s:%s\n", s);
    for (np = (type == Symbol ? symbols[hash(s)] : macros[hash(s)]); np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

Item *install(char *name, ItemType type)
{
    unsigned hashval;
    Item *np;
    int nameLength = strlen(name);

    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        memcpy(np->name, name, nameLength);
        np->name[nameLength] = '\0';
        if (np == NULL || np->name == NULL)
        {
            printf("Memory allocation failed\n");
            return NULL;
        }
        np->next = (Item *)malloc(sizeof(Item *));
        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }
    /* Key name already exist inside table, yield Error of duplicate values in symbol/macro table*/
    else
    {
        printf("Key name already exist inside table!\n");
        return NULL;
    }

    return np;
}

void printSymbolTable()
{
    int i = 0;
    printf("name\tvalue\tbase\toffset\tattributes");
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            printSymbolItem(symbols[i]);
        i++;
    }
}

void printSymbolItem(Item *item)
{
    printf("\n%s\t%u\t%u\t%u\t", item->name, item->val.s.value, item->val.s.base, item->val.s.offset);
    if ((item->val.s.attrs.code || item->val.s.attrs.data) && (item->val.s.attrs.entry || item->val.s.attrs.external))
    {
        if (item->val.s.attrs.code)
            printf("code,");
        else
            printf("data,");

        if (item->val.s.attrs.entry)
            printf("entry");
        else
            printf("external");
    }
    else
    {
        if (item->val.s.attrs.code)
            printf("code");
        else if (item->val.s.attrs.data)
            printf("data");
        else if (item->val.s.attrs.entry)
            printf("entry");
        else
            printf("external");
    }

    if (item->next != NULL)
        printSymbolItem(item->next);
}

void setSymbolData(Item *symbol, unsigned value, Attributes attrs)
{
    unsigned base;
    unsigned offset;
    /* calculate base and offset from value*/
    /*meanwhile I will put some generic values in those variables*/
    base = 96;
    offset = 4;
    symbol->val.s.value = value;
    symbol->val.s.base = base;
    symbol->val.s.offset = offset;
    symbol->val.s.attrs.code = attrs.code;
    symbol->val.s.attrs.entry = attrs.entry;
    symbol->val.s.attrs.external = attrs.external;
    symbol->val.s.attrs.data = attrs.data;
}

void setMacroData(Item *macro, char *code)
{
    macro->val.m.code = code;
}
