#include "table.h"

Item *symbols[HASHSIZE];
Item *macros[HASHSIZE];

extern Flag verifyLabelNaming(char *s);
Flag addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
unsigned hash(char *s);
Item *lookup(char *s, ItemType type);
Flag install(char *name, ItemType type);
void printSymbolTable();
void printSymbolItem(Item *item);
Flag addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
Flag updateSymbol(char *name, int newValue);
Flag setSymbolData(Item *symbol, unsigned value, Attributes attrs);
Flag getMacroCodeValue(char *s);
Flag addMacro(char *name, char *code);

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

Flag install(char *name, ItemType type)
{
    unsigned hashval;
    Flag np;
    int nameLength = strlen(name);
    if ((np.item = lookup(name, (type == Symbol ? Symbol : Macro))) == NULL)
    {
        np.item = (Item *)malloc(sizeof(Item *));
        np.item->name = (char *)malloc(nameLength * sizeof(char));
        memcpy(np.item->name, name, nameLength);
        np.item->name[nameLength] = '\0';
        if (np.item == NULL || np.item->name == NULL)
        {
            printf("Memory allocation failed\n");
            np.err = memoryAllocationFailure;
            return np;
        }
        np.item->next = (Item *)malloc(sizeof(Item *));
        hashval = hash(name);
        np.item->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np.item;
        else
            macros[hashval] = np.item;
    }
    /* Key name already exist inside table, yield Error of duplicate values in symbol/macro table,
    if it is an entry or an external do not yield error
    */
    else
        np.err = type == Macro ? illegalMacroNameAlreadyInUse : illegalSymbolNameAlreadyInUse;

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

Flag addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    Item *p;
    unsigned base;
    unsigned offset;
    Flag result = verifyLabelNaming(name);
    if (result.err)
        return result;
    result = install(name, Symbol);
    if (!result.item)
        return result;

    p = result.item;
    offset = value % 16;
    base = value - offset;
    p->val.s.value = value;
    p->val.s.base = base;
    p->val.s.offset = offset;
    p->val.s.attrs.code = isCode ? 1 : 0;
    p->val.s.attrs.entry = isEntry ? 1 : 0;
    p->val.s.attrs.external = isExternal ? 1 : 0;
    p->val.s.attrs.data = isData ? 1 : 0;

    return result;
}

Flag updateSymbol(char *name, int newValue)
{
    Item *p;
    Flag result;
    unsigned base;
    unsigned offset;
    p = lookup(name, Symbol);
    if (p)
    {
        offset = newValue % 16;
        base = newValue - offset;
        p->val.s.offset = offset;
        p->val.s.base = base;
        p->val.s.value = newValue;
        result.state = True;
    }
    else
        result.err = symbolDoesNotExist;

    return result;
}

Flag getMacroCodeValue(char *s)
{
    Flag result;
    result.item = lookup(s, Macro);
    if (result.item)
        result.text = strdup(result.item->val.m.code);
    else
        result.err = macroDoesNotExist;

    return result;
}

Flag addMacro(char *name, char *code)
{
    Flag result = install(name, Macro);
    if (result.item)
        result.item->val.m.code = strdup(code);
    return result;
}
