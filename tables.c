#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Item* symbols[HASHSIZE];
extern Item* macros[HASHSIZE];
extern const char* regs[REGS_SIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern unsigned getDC();
extern unsigned getIC();
extern unsigned getICF();

void initTablesArrays()
{
    int i = 0;

    while (i < HASHSIZE)
    {
        symbols[i] = NULL;
        macros[i] = NULL;
        i++;
    }
}

unsigned hash(char* s)
{
    unsigned hashval = 1;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

Item* lookup(char* s, ItemType type)
{
    Item* np;
    int i = hash(s);
    for (np = (type == Symbol ? symbols[i] : macros[i]); np != NULL; np = np->next)
        if (!strcmp(s, np->name))
            return np;

    return NULL;
}

Item* install(char* name, ItemType type)
{
    unsigned hashval;
    Item* np;
    np = (Item*)malloc(sizeof(Item*));
    np->name = calloc(strlen(name) + 1, sizeof(char*));
    if (np == NULL || np->name == NULL)
    {
        yieldError(memoryAllocationFailure);
        return NULL;
    }
    else
    {
        memcpy(np->name, name, strlen(name));
        if (type == Symbol)
        {
            np->val.s.attrs.code = 0;
            np->val.s.attrs.entry = 0;
            np->val.s.attrs.external = 0;
            np->val.s.attrs.data = 0;
            np->val.s.base = 0;
            np->val.s.value = 0;
            np->val.s.offset = 0;
        }
        else if (type == Macro)
        {
            np->val.m.start = -1;
            np->val.m.end = -1;
        }

        np->next = (Item*)malloc(sizeof(Item*));
        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }

    return np;
}

void printSymbolTable()
{
    int i = 0;

    printf("\n\t\t ~ SYMBOL TABLE ~ \n");
    printf("name\tvalue\tbase\toffset\tattributes");
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            printSymbolItem(symbols[i]);
        i++;
    }
    printf("\n\n");
}

int printSymbolItem(Item* item)
{
    /*  printf("line 94, inside printSymbolItem \n");
     */
    printf("\n%s\t%u\t%u\t%u\t", item->name, item->val.s.value, item->val.s.base, item->val.s.offset);
    if (!item->val.s.attrs.code && !item->val.s.attrs.data && !item->val.s.attrs.entry && !item->val.s.attrs.external)
        printf("   ");

    else
    {
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
    }
    /*   printf("line 128, table.c \n");
     */
    if (item->next != NULL)
        printSymbolItem(item->next);
    return 0;
}

Bool addSymbol(char* name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    unsigned base;
    unsigned offset;
    Item* p;

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';
    if (!verifyLabelNamingAndPrintErrors(name))
        return False;
    p = lookup(name, Symbol);
    if (p != NULL)
        updateSymbol(p, value, isCode, isData, isEntry, isExternal);
    else
    {
        p = install(name, Symbol);
        offset = value % 16;
        base = abs((unsigned)value - offset);
        p->val.s.value = value;
        p->val.s.base = base;
        p->val.s.offset = offset;
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
    }

    return True;
}

Bool updateSymbol(Item* p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    /*     printf("inside updateSymbol\n");
        printf("name:%s value:%d isCode:%u isData:%u isEntry:%u isExternal:%u\n", p->name, value, isCode, isData, isEntry, isExternal);
     */
     /*     printf("inside updateSymbol\n");
      */
    if (p->val.s.attrs.external && isExternal && (value || isData || isEntry || isCode))
        return yieldError(illegalOverrideOfExternalSymbol);

    else if (p->val.s.attrs.entry && isExternal)
        return yieldError(symbolCannotBeBothCurrentTypeAndRequestedType);
    else
    {
        if ((isData && isCode) || (isCode && p->val.s.attrs.data) || (isData && p->val.s.attrs.code))
            return yieldError(illegalSymbolNameAlreadyInUse);

        if (value)
        {
            unsigned base = 0;
            unsigned offset = 0;
            printf("base: %u offset: %u \n", base, offset);
            offset = value % 16;
            base = abs((unsigned)value - offset);
            p->val.s.value = value;
            p->val.s.base = base;
            p->val.s.offset = offset;
        }

        if (isEntry)
            p->val.s.attrs.entry = 1;
        if (isCode)
            p->val.s.attrs.code = 1;
        if (isData)
            p->val.s.attrs.data = 1;
    }

    return True;
}

Item* getSymbol(char* name)
{
    return lookup(name, Symbol);
}

int getSymbolBaseAddress(char* name)
{
    Item* p = lookup(name, Symbol);
    if (p == NULL)
        return -1;

    return p->val.s.base;
}

int getSymbolOffset(char* name)
{
    Item* p = lookup(name, Symbol);
    if (p == NULL)
        return -1;

    return p->val.s.offset;
}

Bool isExternal(char* name)
{
    Item* p = lookup(name, Symbol);
    if (p == NULL)
        return -1;
    return p->val.s.attrs.external;
}

Bool isLabelNameAlreadyTaken(char* name, ItemType type)
{
    Item* p = lookup(name, type);
    if (p != NULL)
    {
        if (type == Symbol)
        {
            if (p->val.s.attrs.data || p->val.s.attrs.code)
                return True;
            if (p->val.s.attrs.entry)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.external) ? False : True;
            if (p->val.s.attrs.external)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.entry) ? False : True;
        }

        else if (type == Macro)
            return p->val.m.start != -1 ? False : True;
    }

    return False;
}

Item* removeFromTable(char* name, ItemType type)
{
    Item* p = lookup(name, type);
    if (p->next)
        p = p->next;
    else
        p = NULL;

    return p;
}

Item* updateSymbolAddressValue(char* name, int newValue)
{
    Item* p = getSymbol(name);
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
        yieldError(symbolDoesNotExist);

    return p;
}

Item* getMacro(char* s)
{
    Item* p = lookup(s, Macro);
    if (p == NULL)
        yieldError(macroDoesNotExist);
    return p;
}

Item* addMacro(char* name, int start, int end)
{
    Item* macro = lookup(name, Macro);
    if (macro != NULL)
    {
        yieldError(illegalMacroNameAlreadyInUse);
        return NULL;
    }
    else
    {
        macro = install(name, Macro);
        macro->val.m.start = start;
        macro->val.m.end = end;
    }

    return macro;
}

Bool verifyLabelNaming(char* s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return False;

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
        return False;

    if (s[0] == 'r' && labelLength >= 2 && labelLength <= 3)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return False;

            i++;
        }
    }

    else if ((labelLength >= 3 && labelLength <= 4))
    {
        while (i < OP_SIZE)
        {
            if ((strcmp(operations[i].keyword, s) == 0))
                return False;

            i++;
        }
    }
    else
    {

        while (i < labelLength)
        {
            if (!isalnum(s[i]))
                return False;

            i++;
        }
    }

    return True;
}

Bool verifyLabelNamingAndPrintErrors(char* s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return yieldError(illegalLabelNameUseOfCharacters);

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
        return yieldError(illegalLabelNameLength);

    if (s[0] == 'r' && labelLength >= 2 && labelLength <= 3)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return yieldError(illegalLabelNameUseOfSavedKeywords);

            i++;
        }
    }

    else if ((labelLength >= 3 && labelLength <= 4))
    {
        while (i < OP_SIZE)
        {
            if ((strcmp(operations[i].keyword, s) == 0))
                return yieldError(illegalLabelNameUseOfSavedKeywords);

            i++;
        }
    }
    else
    {

        while (i < labelLength)
        {
            if (!isalnum(s[i]))
                return yieldError(illegalLabelNameUseOfCharacters);

            i++;
        }
    }

    return True;
}

void updateFinalMemoryAddressesInSymbolTable()
{
    int i = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            updateSingleItemAddress(symbols[i]);
        i++;
    }
}

int updateSingleItemAddress(Item* item)
{
    if (item->val.s.attrs.data)
    {
        unsigned base = 0, offset = 0, newValue = item->val.s.value + getICF();
        offset = newValue % 16;
        base = newValue - offset;
        item->val.s.offset = offset;
        item->val.s.base = base;
        item->val.s.value = newValue;
    }

    if (item->next != NULL)
        updateSingleItemAddress(item->next);
    return 0;
}