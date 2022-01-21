#include "table.h"

Item *symbols[HASHSIZE];
Item *macros[HASHSIZE];

extern State globalState;
extern Error currentError;

void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Item *install(char *name, ItemType type);
void printSymbolTable();
void printSymbolItem(Item *item);
void addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
void updateSymbol(char *name, int newValue);
Flag setSymbolData(Item *symbol, unsigned value, Attributes attrs);
char *getMacroCodeValue(char *s);
void addMacro(char *name, char *code);
void verifyLabelNaming(char *s);

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
    verifyLabelNaming(name);
    if (globalState == collectErrors)
        NULL;
    if ((np = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np = (Item *)malloc(sizeof(Item *));
        np->name = (char *)malloc(nameLength * sizeof(char));
        memcpy(np->name, name, nameLength);
        np->name[nameLength] = '\0';
        if (np == NULL || np->name == NULL)
        {
            printf("Memory allocation failed\n");
            globalState = collectErrors;
            currentError = memoryAllocationFailure;
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
        globalState = collectErrors;
        currentError = type == Macro ? illegalMacroNameAlreadyInUse : illegalSymbolNameAlreadyInUse;
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
    Item *p;
    unsigned base;
    unsigned offset;
    p = install(name, Symbol);
    if (globalState != collectErrors)
    {
        offset = value % 16;
        base = value - offset;
        p->val.s.value = value;
        p->val.s.base = base;
        p->val.s.offset = offset;
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
    }
}

void updateSymbol(char *name, int newValue)
{
    Item *p = lookup(name, Symbol);
    unsigned base;
    unsigned offset;

    if (p != NULL)
    {
        offset = newValue % 16;
        base = newValue - offset;
        p->val.s.offset = offset;
        p->val.s.base = base;
        p->val.s.value = newValue;
    }
    else
    {
        globalState = collectErrors;
        currentError = symbolDoesNotExist;
    }
}

char *getMacroCodeValue(char *s)
{
    char macrosCode = lookup(s, Macro);
    if (macrosCode != NULL)
        return strdup(result.item->val.m.code);
    else
    {
        globalState = collectErrors;
        currentError = macroDoesNotExist;
    }

    return NULL;
}

void addMacro(char *name, char *code)
{
    Item *item = install(name, Macro);
    if (globalState != collectErrors)
        item->val.m.code = strdup(code);
}
