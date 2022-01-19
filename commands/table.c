#include "table.h"
Item *symbols[HASHSIZE];
Item *macros[HASHSIZE];

Command commands[] = {
    /*  */
    {0x0001, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

Command *getCommandByName(char *s)
{
    int i = 0;
    int length = (int)(sizeof(commands) / sizeof(commands[0]));
    while (i < length)
    {
        if (strcmp(commands[i].keyword, s) == 0)
            return &commands[i];
        i++;
    }
    return NULL;
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
    int nameLength = strlen(name);
    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        np->name = (char *)malloc(nameLength * sizeof(char));
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
    /* Key name already exist inside table, yield Error of duplicate values in symbol/macro table,
    if it is an entry or an external do not yield error
    */
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
    printf("\n");
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

void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    Attributes att;
    att.data = isData ? 1 : 0;
    att.code = isCode ? 1 : 0;
    att.entry = isEntry ? 1 : 0;
    att.external = isExternal ? 1 : 0;
    setSymbolData(install(name, Symbol), value, att);
}

void setSymbolData(Item *symbol, unsigned value, Attributes attrs)
{
    unsigned base;
    unsigned offset;
    offset = value % 16;
    base = value - offset;
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
